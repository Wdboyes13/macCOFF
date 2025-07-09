#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream) {
    if (!lineptr || !n || !stream) return -1;

    size_t pos = 0;
    int c;

    if (*lineptr == NULL || *n == 0) {
        *n = 128; // start buffer size
        *lineptr = malloc(*n);
        if (!*lineptr) return -1;
    }

    while ((c = fgetc(stream)) != EOF) {
        // grow buffer if needed
        if (pos + 1 >= *n) {
            size_t new_size = *n * 2;
            char *new_ptr = realloc(*lineptr, new_size);
            if (!new_ptr) return -1;
            *lineptr = new_ptr;
            *n = new_size;
        }

        (*lineptr)[pos++] = (char)c;
        if (c == '\n') break;  // stop at newline
    }

    if (pos == 0 && c == EOF) return -1; // EOF and no data read

    (*lineptr)[pos] = '\0'; // null terminate

    return pos; // return length including newline
}