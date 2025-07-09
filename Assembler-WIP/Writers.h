#pragma once

#include <stdio.h>
#include <stdint.h>
#include "../privinc/FileHeader.h"
#include "../privinc/SectionHeader.h"

void write_le16(FILE* f, uint16_t val);
void write_le64(FILE* f, uint64_t val);
void write_header(FILE* f, COFFHeader* h);
void write_sect(FILE* f, COFFSectionHeader* h);