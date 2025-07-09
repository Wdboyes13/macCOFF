#pragma once

typedef struct SymtabEntry {
    char n_name[8];
    long n_value;
    short n_scnum;
    unsigned short n_type;
    char n_sclass;
    char n_numaux;
} SymtabEntry;

#define C_EXT 2
#define C_STAT 3