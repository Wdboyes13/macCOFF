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

SectionHeader section @ (sizeof(CoffHeader) + 0 * sizeof(SectionHeader));

struct TextData {
    u32 instruction[section.s_size / 4];
};

TextData text @ ( sizeof(CoffHeader) + 0 * sizeof(SectionHeader) + section.s_scnptr);