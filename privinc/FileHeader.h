#pragma once

#pragma pack(push, 1)
typedef struct COFFHeader {
	unsigned short 	f_magic;	/* Magic number */	
	unsigned short 	f_nscns;	/* Number of Sections */
	long 		f_timdat;	/* Time & date stamp */
	long 		f_symptr;	/* File pointer to Symbol Table */
	long 		f_nsyms;	/* Number of Symbols */
	unsigned short 	f_opthdr;	/* sizeof(Optional Header) */
	unsigned short 	f_flags;	/* Flags */
} COFFHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct COFFOpthdr {
	unsigned short 	magic;          /* Magic Number                    */
	unsigned short 	vstamp;         /* Version stamp                   */
	unsigned long 	tsize;          /* Text size in bytes              */
	unsigned long 	dsize;          /* Initialised data size           */
	unsigned long 	bsize;          /* Uninitialised data size         */
	unsigned long 	entry;          /* Entry point                     */
	unsigned long 	text_start;     /* Base of Text used for this file */
	unsigned long 	data_start;     /* Base of Data used for this file */
} COFFOpthdr;
#pragma pack(pop)

#define MCOFF_MAGIC 0x9f56

#define MC_EXEC 0x01
#define MC_TST 0x00