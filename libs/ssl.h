//
// Created by Baptiste Oger on 19/08/2024.
//

#ifndef SSL_H
#define SSL_H

#include <openssl/ssl.h>
#include <openssl/err.h>

// Initializing OpenSSL
void initialize_openssl();

// Cleaning OpenSSL
void cleanup_openssl();

// Creating an SSL context
SSL_CTX *create_context(int is_server);

// Configuring the SSL context (only for the server)
void configure_context(SSL_CTX *ctx);

#endif //SSL_H
