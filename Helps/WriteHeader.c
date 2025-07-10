#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "../include/FileHeader.h"
#include "../include/SectionHeader.h"

void write_le16(FILE* f, uint16_t val) {
    uint8_t b[2] = { val & 0xff, (val >> 8) & 0xff };
    fwrite(b, 1, 2, f);
}

void write_le64(FILE* f, uint64_t val) {
    uint8_t b[8];
    for (int i = 0; i < 8; i++) {
        b[i] = (val >> (8 * i)) & 0xff;
    }
    fwrite(b, 1, 8, f);
}

void write_header(FILE* f, COFFHeader* h) {
    write_le16(f, h->f_magic);
    write_le16(f, h->f_nscns);
    write_le64(f, h->f_timdat);
    write_le64(f, h->f_symptr);
    write_le64(f, h->f_nsyms);
    write_le16(f, h->f_opthdr);
    write_le16(f, h->f_flags);
}

void write_sect(FILE* f, COFFSectionHeader* h) {
    fwrite(h->s_name, 1, 8, f);
    write_le64(f, h->s_paddr);
    write_le64(f, h->s_vaddr);
    write_le64(f, h->s_size);
    write_le64(f, h->s_scnptr);
    write_le64(f, h->s_relptr);
    write_le64(f, h->s_lnnoptr);
    write_le16(f, h->s_nreloc);
    write_le16(f, h->s_nlnno);
    write_le64(f, h->s_flags);
}


int main() {
    FILE* CFile = fopen("tst.coff", "wb");
    if (!CFile) {
        perror("Failed to open output file");
        return 1;
    }

    COFFHeader header = {
        .f_magic = MCOFF_MAGIC,
        .f_nscns = 1,
        .f_timdat = (uint64_t)time(NULL),
        .f_symptr = 0,      // Not used
        .f_nsyms = 0,
        .f_opthdr = 0,
        .f_flags = MC_EXEC
    };

    COFFSectionHeader secthead = {0}; // zero everything first
    memset(secthead.s_name, 0, 8);                   // Zero all 8 bytes
    memcpy(secthead.s_name, ".text", strlen(".text"));  // Copy only 5 characters
    secthead.s_size = 32; // bytes of code
    secthead.s_scnptr = ftell(CFile) + sizeof(COFFSectionHeader); // offset of code
    secthead.s_flags = STYP_TEXT;

    uint8_t ExecData[] = {
        0x01, 0x00, 0x80, 0xD2, // movz x0, #1
        0x41, 0x00, 0x80, 0xD2, // movz x1, #65
        0x22, 0x00, 0x80, 0xD2, // movz x2, #1
        0x90, 0x00, 0x80, 0xD2, // movz x16, #4
        0x00, 0x00, 0x00, 0xD4, // svc
        0x00, 0x00, 0x80, 0xD2, // movz x0, #0
        0x30, 0x00, 0x80, 0xD2, // movz x16, #1
        0x00, 0x00, 0x00, 0xD4  // svc #0
    };

    write_header(CFile, &header);
    write_sect(CFile, &secthead);
    fwrite(ExecData, sizeof(ExecData), 1, CFile);

    fclose(CFile);
    printf("Wrote: %lu bytes\n", sizeof(header) + sizeof(secthead) + sizeof(ExecData));
    return 0;
}