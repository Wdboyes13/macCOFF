#pragma once

#include "FileHeader.h"

#pragma pack(push, 1)
typedef struct COFFSectionHeader {
	char		s_name[8];	/* Section Name */
	long		s_paddr;	/* Physical Address */
	long		s_vaddr;	/* Virtual Address */
	long		s_size;		/* Section Size in Bytes */
	long		s_scnptr;	/* File offset to the Section data */
	long		s_relptr;	/* File offset to the Relocation table for this Section */
	long		s_lnnoptr;	/* File offset to the Line Number table for this Section */
	unsigned short	s_nreloc;	/* Number of Relocation table entries */
	unsigned short	s_nlnno;	/* Number of Line Number table entries */
	long		s_flags;	/* Flags for this section */
} COFFSectionHeader;
#pragma pack(pop)

#define STYP_TEXT 0x0020
#define STYP_DATA 0x0040
#define STYP_BSS 0x0080

void LoadSections(unsigned char* Exec, COFFHeader header);