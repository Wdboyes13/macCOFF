#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/FileHeader.h"
#include "../include/SectionHeader.h"
#include "../include/Helpers.h"

#define Set16(Field, Text) \
    Field = read_le16(&Exec[index]); \
    index += 2; \
    printf("%s: %u\n", Text, Field)

#define Set64(Field, Text) \
    Field = read_le64(&Exec[index]); \
    index += 8; \
    printf("%s: %lu\n", Text, Field)

void COFFHeadLoad(unsigned char* Exec) {
    int index = 0;
    COFFHeader DecHeader;

    // COFF magic is little endian: 0x56 0x9f
    if (!(Exec[index] == 0x56 && Exec[index + 1] == 0x9f)) {
        fprintf(stderr, "Invalid Magic - Expected 0x569F Got 0x%02x%02x\n", Exec[0], Exec[1]);
        return;
    }
    index += 2;

    Set16(DecHeader.f_nscns, "Number of sections");
    Set64(DecHeader.f_timdat, "Timestamp");
    Set64(DecHeader.f_symptr, "Symbol Table Pointer (Unused)");
    Set64(DecHeader.f_nsyms, "Number of symbols (Unused)");
    Set16(DecHeader.f_opthdr, "Optional Header Size");
    Set16(DecHeader.f_flags, "Exec type");

    if (DecHeader.f_opthdr > 0){
        COFFOpthdr DecOpthdr;
        if (!(Exec[index] == 0x56 && Exec[index + 1] == 0x9f)) {
            fprintf(stderr, "Invalid Opthdr Magic - Expected 0x569F Got 0x%02x%02x\n", Exec[0], Exec[1]);
            return;
        }
        index += 2;

        Set16(DecOpthdr.vstamp, "Version");
        Set64(DecOpthdr.tsize, "Text Size (Unused)");
        Set64(DecOpthdr.dsize, "Data Size (Unused)");
        Set64(DecOpthdr.bsize, "BSS Size (Unused)");
        Set64(DecOpthdr.entry, "Entry Point (Unused)");
        Set64(DecOpthdr.text_start, "Text Data Start (Unused)");
        Set64(DecOpthdr.data_start, "Data Start (Unused)");
    }

    LoadSections(Exec, DecHeader);
}