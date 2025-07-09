#include "../privinc/SectionHeader.h"
#include "../privinc/FileHeader.h"
#include "../privinc/Helpers.h"
#include "../Executor/Executor.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


COFFSectionHeader readSec(unsigned char* Exec, long offset){
        int index = offset;
        COFFSectionHeader DecHeader;
            // --- Read section name (8 bytes)
        memcpy(DecHeader.s_name, &Exec[index], 8);
        index += 8;
        if (memchr(DecHeader.s_name, '\0', 8) == NULL)
            DecHeader.s_name[7] = '\0'; 
        printf("Section Name: %s\n", DecHeader.s_name);

        // --- Read fields
        DecHeader.s_paddr    = read_le64(&Exec[index]); index += 8;
        DecHeader.s_vaddr    = read_le64(&Exec[index]); index += 8;
        DecHeader.s_size     = read_le64(&Exec[index]); index += 8;
        DecHeader.s_scnptr   = read_le64(&Exec[index]); index += 8;
        DecHeader.s_relptr   = read_le64(&Exec[index]); index += 8;
        DecHeader.s_lnnoptr  = read_le64(&Exec[index]); index += 8;
        DecHeader.s_nreloc   = read_le16(&Exec[index]); index += 2;
        DecHeader.s_nlnno    = read_le16(&Exec[index]); index += 2;
        DecHeader.s_flags    = read_le64(&Exec[index]); index += 8;

        // --- Print summary
        printf("  Size:   0x%lx bytes\n", DecHeader.s_size);
        printf("  Offset: 0x%lx bytes into file\n", DecHeader.s_scnptr);
        printf("  Flags:  0x%08lx\n\n", DecHeader.s_flags);
        printf("Dumping %s section:\n", DecHeader.s_name);
        if (strcmp(DecHeader.s_name, ".text") == 0) {
        for (int i = 0; i < DecHeader.s_size; i += 4) {
                uint32_t instr = *(uint32_t*)(&Exec[DecHeader.s_scnptr + i]);
                printf("  @%02x: 0x%08x\n", i, instr);
            }
        } else if (strcmp(DecHeader.s_name, ".data") == 0) {
            printf("  Raw bytes: ");
            for (int i = 0; i < DecHeader.s_size; i++) {
                printf("%02x ", Exec[DecHeader.s_scnptr + i]);
            }
            printf("\n");
        }
    return DecHeader;
}

void LoadSections(unsigned char* Exec, COFFHeader header) {
    COFFSectionHeader DecHeader;
    COFFSectionHeader DatHeader;
    if (header.f_opthdr > 0){
        DecHeader = readSec(Exec, sizeof(COFFHeader) + 0 * sizeof(COFFSectionHeader) + header.f_opthdr);
        DatHeader = readSec(Exec, sizeof(COFFHeader) + 1 * sizeof(COFFSectionHeader) + header.f_opthdr);
    } else {
        DecHeader = readSec(Exec, sizeof(COFFHeader) + 0 * sizeof(COFFSectionHeader));
        DatHeader = readSec(Exec, sizeof(COFFHeader) + 1 * sizeof(COFFSectionHeader));
    }
    uint8_t* DatSect = malloc(DatHeader.s_size);
    if (!DatSect) {
        perror("malloc failed");
        exit(1);
    }
    memcpy(DatSect, &Exec[DatHeader.s_scnptr], DatHeader.s_size);
    ExecMain(&Exec[DecHeader.s_scnptr], DecHeader.s_size, DecHeader.s_scnptr, DatSect, DatHeader.s_size);
    free(DatSect);
}