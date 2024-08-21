#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../libs/hash.h"
#include "../libs/ssl.h"

int main(int argc, char *argv[]) {
    // Get IP, PORT from arguments
    int opt;
    char *ip = NULL;
    int port = 0;
    char filename[256] = "";

    while ((opt = getopt(argc, argv, "i:p:f:")) != -1) {
        switch (opt) {
            case 'i':
                ip = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'f':
                strncpy(filename, optarg, sizeof(filename) - 1);
                filename[sizeof(filename) - 1] = '\0';
                break;
            default:
                fprintf(stderr, "Usage: %s -i <ip> -p <port> -f <filename>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Check if the IP, port and filename are set
    if (ip == NULL || port == 0 || strlen(filename) == 0) {
        fprintf(stderr, "An IP, a port and a filename must be specified\n");
        exit(EXIT_FAILURE);
    }

    // Create an SSL socket
    SSL_CTX *ctx;
    SSL *ssl;
    int server;

    struct sockaddr_in addr;

    initialize_openssl();
    ctx = create_context(0);

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(server, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to connect");
        exit(EXIT_FAILURE);
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, server);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));

        // Sending the file name to the server
        if ( SSL_write(ssl, filename, strlen(filename)) > 0 ) {
            printf("[+] File name sent to the server\n");
        } else {
            perror("[—] Error while sending the file name");
            exit(EXIT_FAILURE);
        }

        // Receiving the file from the server
        FILE *file = fopen("received_file", "wb");
        if (file == NULL) {
            perror("[–] Failed to open file");
            exit(EXIT_FAILURE);
        }

        char buffer[1024];
        int file_bytes;
        while ((file_bytes = SSL_read(ssl, buffer, sizeof(buffer))) > 0) {
            if (strncmp(buffer, "EOF", 3) == 0) {
                break;
            }
            fwrite(buffer, 1, file_bytes, file);
        }
        fclose(file);
        printf("[+] File received\n");

        //Receiving the hash from the server
        char hash[1024];
        if ( SSL_read(ssl, hash, sizeof(hash) - 1) > 0 ) {
            printf("[+] Hash received\n");
        } else {
            perror("[–] Error while receiving the hash");
            exit(EXIT_FAILURE);
        }

        // Calculate the hash of the received file
        char calculated_hash[1024];
        calculate_md5("received_file", calculated_hash);

        if (calculated_hash == NULL) {
            perror("[–] Error while calculating the hash");
            exit(EXIT_FAILURE);
        }
        printf("[+] Hash calculated successfully\n");

        printf("Server hash : %s\n", hash);
        printf("Calculated hash : %s\n", calculated_hash);

        // Compare the hash received from the server and the calculated hash
        if (strcmp(hash, calculated_hash) == 0) {
            printf("[+] Hashes match :)\n");
            printf("The file is authentic\n");
        } else {
            printf("[–] Hashes don't match :(\n");
            printf("The file is not authentic\n");
        }
    }

    SSL_free(ssl);
    close(server);
    SSL_CTX_free(ctx);
    cleanup_openssl();

    return 0;
}
