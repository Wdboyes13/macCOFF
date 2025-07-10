#pragma once

#include "../include/SectionHeader.h"
#include <stdint.h>

void ExecMain(unsigned char* Exec, long s_size, long file_offset, uint8_t* DataSect, long DataSize);