#include <stdio.h>
#include <stdint.h>
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