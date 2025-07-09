#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../privinc/FileHeader.h"
#include "Writers.h"

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

int main(int argc, char* argv[]){
    if (argc < 3){
        printf("Invalid Command\n");
        printf("Syntax: %s [Input.s] [Output]\n", argv[0]);
        exit(1);
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    FILE *o = fopen(argv[2], "wb");
    if (!o) {
        perror("fopen");
        fclose(f);
        return 1;
    }

    COFFHeader header = {
        .f_magic = MCOFF_MAGIC,
        .f_nscns = 0,
        .f_timdat = time(NULL),
        .f_symptr = 0,
        .f_nsyms = 0,
        .f_opthdr = 0,
        .f_flags = MC_EXEC
    };

    COFFSectionHeader* SectHeaders;
    SectHeaders = (COFFSectionHeader*)malloc(sizeof(COFFSectionHeader) * 2);

    char *line = NULL;
    size_t len = 0;
    while (my_getline(&line, &len, f) != -1) {
        if (strcmp(line, ".section .text")){
            header.f_nscns++;
            memset(SectHeaders[0].s_name, 0, 8);  
            memcpy(SectHeaders[0].s_name, ".text", strlen(".text"));
            SectHeaders->s_flags = STYP_TEXT;
        }
    }

    free(line);

    write_header(o, &header);

    fclose(f);
    return 0;
}