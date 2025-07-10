#include <stdio.h>
#include <stdlib.h>
#include "../include/macCOFF.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <coff-binary>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate buffer
    unsigned char *buffer = malloc(file_size);
    if (!buffer) {
        perror("Failed to allocate buffer");
        fclose(file);
        return 1;
    }

    // Read file into buffer
    size_t read = fread(buffer, 1, file_size, file);
    if (read != file_size) {
        fprintf(stderr, "Error reading file\n");
        free(buffer);
        fclose(file);
        return 1;
    }

    fclose(file);

    // Parse
    COFFHeadLoad(buffer);

    free(buffer);
    return 0;
}