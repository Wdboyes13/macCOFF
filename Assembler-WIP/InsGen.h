#pragma once

#include <stdint.h>

uint32_t get_movz_bytes(uint8_t rd, uint16_t imm16, uint8_t hw);
uint32_t get_movk_bytes(uint8_t rd, uint16_t imm16, uint8_t hw);
uint32_t get_svc();