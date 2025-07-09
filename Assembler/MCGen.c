#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Writers.h"
#include "Readers.h"
#include "InsGen.h"
#include <stdbool.h>

int count_lines_until_data(FILE *f) {
    int lines = 0;
    char* line = NULL;
    size_t len = 0;

    rewind(f);  // Make sure we're at the start
    while (getline(&line, &len, f) != -1) {
        // Trim newline
        line[strcspn(line, "\r\n")] = 0;

        if (strcmp(line, ".section .data") == 0 || strcmp(line, ".data") == 0) {
            break;  // Stop at start of .data
        }
        lines++;
    }

    free(line);
    return lines;
}

int count_lines_after_data(FILE *f) {
    int lines = 0;
    char* line = NULL;
    size_t len = 0;

    rewind(f);
    int found_data = 0;

    while (getline(&line, &len, f) != -1) {
        line[strcspn(line, "\r\n")] = 0;

        if (found_data) {
            lines++;
        }

        if (strcmp(line, ".section .data") == 0 || strcmp(line, ".data") == 0) {
            found_data = 1;
        }
    }

    free(line);
    return lines;
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

    COFFHeader FHeader = {
        .f_magic = MCOFF_MAGIC,
        .f_nscns = 2,
        .f_timdat = time(NULL),
        .f_symptr = 0,
        .f_nsyms = 0,
        .f_opthdr = 0,
        .f_flags = MC_EXEC
    };

    int bytes = 0;
    int data_size = 0;

    write_header(o, &FHeader);

    // Save file position right here — this is where section headers start
    long section_headers_offset = ftell(o);

    // Write two empty section headers as placeholders
    COFFSectionHeader empty = {0};
    fwrite(&empty, sizeof(empty), 1, o);
    fwrite(&empty, sizeof(empty), 1, o);
     

    const char* delim = " ,";
    char *line = NULL;
    size_t len = 0;
    int currsect = 1; // .text
    bool TextDeclared = false;
    bool DataDeclared = false;

    while (getline(&line, &len, f) != -1) {
        printf("Read line: %s\n", line);
        line[strcspn(line, "\r\n")] = 0;
        if (strcmp(line, "") == 0) continue;
        if (strcmp(line, ".section .data") == 0 || strcmp(line, ".data") == 0){
            if (!DataDeclared){
                currsect = 2; // Switch to context .data
                DataDeclared = true;
            } else {
                fprintf(stderr, "Error: Redeclared section .data");
                exit(1);
            }
            continue;
        }
        if (strcmp(line, ".section .text") == 0 || strcmp(line, ".text") == 0){
            if (!TextDeclared){
                currsect = 1; // Switch to context .text
                TextDeclared = true;
            } else {
                fprintf(stderr, "Error: Redeclared section .text");
                exit(1);
            }
            continue;
        }
        if (currsect == 1){
            char* tok = NULL;
            char* tok1 = NULL;
            char* tok2 = NULL;
            tok = strtok(line, delim);
            if (tok != NULL){
                tok1 = strtok(NULL, delim);
                if (tok1 != NULL){
                    tok2 = strtok(NULL, delim);
                } 
            }
            if (strcmp(tok, "movz") == 0){
                if (tok1 != NULL && tok2 != NULL){
                    uint8_t rd = -1;
                    uint16_t imm16 = -1;
                    if (tok1[0] == 'x'){
                        rd = atoi(tok1 + 1);
                    }
                    if (tok2[0] == '#'){
                        imm16 = (int)strtol(tok2 + 1, NULL, 0);
                    }
                    if (rd >= 0 && imm16 >= 0){
                        uint32_t val = get_movz_bytes(rd, imm16, 0);
                        fwrite(&val, sizeof(val), 1, o);
                        bytes += 4;
                    }
                }
            }
            if (strcmp(tok, "svc") == 0){
                uint32_t val = get_svc();
                fwrite(&val, sizeof(val), 1, o);
                bytes += 4;
            }
        }
        if (currsect == 2){
            char* tok = NULL;
            char* tok1 = NULL;
            tok = strtok(line, ": ");
            if (tok != NULL){
                tok1 = strtok(NULL, "");
            }
            if (tok1 != NULL && strcmp(tok, "valz") == 0){
                tok1[strcspn(tok1, "\r\n")] = 0;
                printf("tok: '%s', tok1: '%s'\n", tok, tok1);
                if (tok1[0] == '"' && tok1[strlen(tok1) - 1] == '"'){
                    char padded[16] = {0}; // zero-initialize all 16 bytes
                // Replace escape sequences in tok1 (like \n → 0x0A)
                for (size_t i = 0; i < strlen(tok1); i++) {
                    if (tok1[i] == '\\') {
                        if (tok1[i+1] == 'n') {
                            tok1[i] = '\n';
                            // Shift the rest left by one to remove the 'n'
                            memmove(&tok1[i + 1], &tok1[i + 2], strlen(&tok1[i + 2]) + 1);
                        }
                    }
                }
                    // Copy the contents inside the quotes (exclude the first and last character)
                    size_t len = strlen(tok1) - 2;
                    if (len > 16) len = 16; // truncate if too long

                    memcpy(padded, tok1 + 1, len); // skip the opening quote
                    
                    // Now write `padded` to file or whatever you're doing
                    fwrite(padded, 1, 16, o); // assuming `o` is your FILE*
                    data_size += 16;
                }
            }
        }
    }

    fflush(o);
    // Seek back to where the section headers start
    fseek(o, section_headers_offset, SEEK_SET);

    // Write .text section header
    COFFSectionHeader secthead = {0};
    memset(secthead.s_name, 0, 8);
    memcpy(secthead.s_name, ".text", strlen(".text"));
    secthead.s_size = bytes;
    secthead.s_scnptr = section_headers_offset + 2 * sizeof(COFFSectionHeader); // after headers
    secthead.s_flags = STYP_TEXT;
    write_sect(o, &secthead);

    // Write .data section header
    COFFSectionHeader datsecthead = {0};
    memset(datsecthead.s_name, 0, 8);
    memcpy(datsecthead.s_name, ".data", strlen(".data"));
    datsecthead.s_size = data_size;
    datsecthead.s_scnptr = secthead.s_scnptr + bytes; // code offset + code size
    datsecthead.s_flags = STYP_DATA;
    write_sect(o, &datsecthead);

    // Seek to end to close properly
    fseek(o, 0, SEEK_END);

    free(line);

    fclose(f);
    return 0;
}