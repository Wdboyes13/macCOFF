#pragma once

#include <stdint.h>

uint64_t read_le64(unsigned char* buf);
uint16_t read_le16(unsigned char* buf);
uint8_t read_le8(unsigned char* buf);