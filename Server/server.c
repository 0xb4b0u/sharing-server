#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../libs/ssl.h"
#include "../libs/hash.h"

int main(int argc, char *argv[]) {

    // Get IP, PORT and hash list from arguments
    int opt;
    char *ip = NULL;
    int port = 0;
    char *hash_list_path = NULL;

    while ((opt = getopt(argc, argv, "i:p:h:")) != -1) {
        switch (opt) {
            case 'i':
                ip = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                hash_list_path = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -i <ip> -p <port> -h <hash_list_path>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (ip == NULL || port == 0 || hash_list_path == NULL) {
        fprintf(stderr, "An IP, a port and the path to the hash list must be specified\n");
        exit(EXIT_FAILURE);
    }

    int sock;
    SSL_CTX *ctx;

    initialize_openssl();
    ctx = create_context(1);
    configure_context(ctx);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[–] Unable to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[–] Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 1) < 0) {
        perror("[–] Unable to listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        struct sockaddr_in addr;
        uint len = sizeof(addr);
        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("[–] Unable to accept the connection");
            exit(EXIT_FAILURE);
        }

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        } else {
            printf("[+] Connection established\n");

            // Receive the filename from the client
            char filename[256];
            int response = SSL_read(ssl, filename, sizeof(filename) - 1);
            if (response > 0) {
                printf("[+] Received filename: %s\n", filename);
            } else {
                perror("[–] Failed to receive the filename");
                exit(EXIT_FAILURE);
            }

            // Sending the file to the client
            FILE *file_to_send = fopen(filename, "r");
            if (file_to_send == NULL) {
                perror("[–] Failed to open file. It might not exist.");
                exit(EXIT_FAILURE);
            }

            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), file_to_send)) > 0) {
                SSL_write(ssl, buffer, bytes_read);
            }
            SSL_write(ssl, "EOF", 3);
            fclose(file_to_send);
            printf("[+] File sent to the client\n");

            //Find the hash of the file in the hash list and send it
            char hash[1024];
            strncpy(hash, read_hash(hash_list_path, filename), sizeof(hash) - 1);
            hash[sizeof(hash) - 1] = '\0';

            if ( SSL_write(ssl, hash, strlen(hash)) ) {
                printf("[+] Hash sent to the client\n");
            } else {
                perror("[–] Failed to send the hash");
                exit(EXIT_FAILURE);
            }
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}
