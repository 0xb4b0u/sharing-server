#include "hash.h"

char* read_hash(const char* list_path, const char* filename) {
    static char hash[MAX_HASH_LENGTH];
    FILE *file = fopen(list_path, "r");

    if (file == NULL) {
        perror("[–] Failed to open the hash list file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Remove the newline character
        line[strcspn(line, "\n")] = 0;

        // Tokenize the line
        char* filename_token = strtok(line, " - ");
        char* hash_token = strtok(NULL, " - ");

        if (filename_token && hash_token && strcmp(filename_token, filename) == 0) {
            // Copy the hash to the hash buffer and return it
            if (strlen(hash_token) < MAX_HASH_LENGTH) {
                strncpy(hash, hash_token, MAX_HASH_LENGTH - 1);
                hash[MAX_HASH_LENGTH - 1] = '\0';
            } else {
                fprintf(stderr, "[–] Hash length exceeds buffer size\n");
                fclose(file);
                return NULL;
            }
            fclose(file);
            return hash;
        }
    }

    fclose(file);
    return NULL; // If not found
}

// Create a function that calculates the MD5 hash of a file.
void calculate_md5(const char *filename, char *md5_str)
{
    // Create a buffer to store the hash.
    unsigned char hash[MD5_DIGEST_LENGTH] = {0};

    // Create a context to store the hash.
    MD5_CTX mdContext;

    // Create a buffer to store the data.
    int bytes;
    unsigned char data[1024];

    // Open the file.
    FILE *inFile = fopen(filename, "rb");
    if (inFile == NULL)
    {
        perror(filename);
        strcpy(md5_str, "ERROR");
        return;
    }

    // Initialize the MD5 context.
    MD5_Init(&mdContext);

    // Read the file in chunks and update the MD5 context.
    while ((bytes = fread(data, 1, 1024, inFile)) != 0)
    {
        MD5_Update(&mdContext, data, bytes);
    }

    // Finalize the MD5 context and store the hash.
    MD5_Final(hash, &mdContext);

    // Convert the hash to a string.
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        sprintf(&md5_str[i * 2], "%02x", hash[i]);
    }

    // Close the file.
    fclose(inFile);
}