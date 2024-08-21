#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

#define MAX_LINE_LENGTH 256
#define MAX_HASH_LENGTH 64

char* read_hash(const char* list_path, const char* filename);
void calculate_md5(const char *filename, char *md5_str);

#endif //HASH_H
