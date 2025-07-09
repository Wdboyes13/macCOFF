// Define the COFF Header type
struct CoffHeader {
    u16 f_magic;
    u16 f_nscns;
    u64 f_timdat;
    u64 f_symptr;
    u64 f_nsyms;
    u16 f_opthdr;
    u16 f_flags;
};

// Instantiate the header from offset 0
CoffHeader header @ 0x00;

struct COFFOpthdr {
    u16  magic;          /* Magic Number                    */
    u16  vstamp;         /* Version stamp                   */
    u64  tsize;          /* Text size in bytes              */
    u64  dsize;          /* Initialised data size           */
    u64  bsize;          /* Uninitialised data size         */
    u64  entry;          /* Entry point                     */
    u64  text_start;     /* Base of Text used for this file */
    u64  data_start;     /* Base of Data used for this file */
};

// Define the Section Header type
struct SectionHeader {
    char s_name[8];
    u64 s_paddr;
    u64 s_vaddr;
    u64 s_size;
    u64 s_scnptr;
    u64 s_relptr;
    u64 s_lnnoptr;
    u16 s_nreloc;
    u16 s_nlnno;
    u64 s_flags;
};

COFFOpthdr opthdr @ sizeof(CoffHeader);

SectionHeader section @ (sizeof(COFFOpthdr) + sizeof(CoffHeader) + 0 * sizeof(SectionHeader));

SectionHeader datsection @ (sizeof(COFFOpthdr) + sizeof(CoffHeader) + 1 * sizeof(SectionHeader));

struct Instruction {
    u32 Instruction;
};

struct DatData {
    char datab;
};

Instruction text[8] @ section.s_scnptr;
DatData dat[datsection.s_size] @ datsection.s_scnptr;