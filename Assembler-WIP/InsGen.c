#include <stdint.h>
#include <stdlib.h>

uint32_t combine_bytes(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
    return ((uint32_t)b0)       |
           ((uint32_t)b1 << 8)  |
           ((uint32_t)b2 << 16) |
           ((uint32_t)b3 << 24);
}

// Generates a MOVZ instruction word for ARM64
uint32_t movz_instruction(uint8_t rd, uint16_t imm16, uint8_t hw) {
    // movz opcode base (top 9 bits 110100101)
    uint32_t opcode = 0xD2800000;

    // shift hw is 2 bits, multiply by 16 for halfword offset
    uint32_t hw_bits = (hw & 0x3) << 21;

    // imm16 is 16 bits, placed at bits 5-20
    uint32_t imm_bits = ((uint32_t)imm16 & 0xFFFF) << 5;

    // rd is bits 0-4
    uint32_t rd_bits = rd & 0x1F;

    return opcode | hw_bits | imm_bits | rd_bits;
}

// Write the instruction as 4 bytes in little endian to stdout
uint32_t get_movz_bytes(uint8_t rd, uint16_t imm16, uint8_t hw) {
    uint32_t instr = movz_instruction(rd, imm16, hw);
    return combine_bytes( instr & 0xFF,
    (instr >> 8) & 0xFF,
    (instr >> 16) & 0xFF,
    (instr >> 24) & 0xFF
    );
}

// Generates a MOVK instruction word for ARM64
uint32_t movk_instruction(uint8_t rd, uint16_t imm16, uint8_t hw) {
    // movk opcode base (top 9 bits 111100101)
    uint32_t opcode = 0xF2800000;

    // shift hw is 2 bits, multiply by 16 for halfword offset
    uint32_t hw_bits = (hw & 0x3) << 21;

    // imm16 is 16 bits, placed at bits 5-20
    uint32_t imm_bits = ((uint32_t)imm16 & 0xFFFF) << 5;

    // rd is bits 0-4
    uint32_t rd_bits = rd & 0x1F;

    return opcode | hw_bits | imm_bits | rd_bits;
}

// Write the instruction as 4 bytes in little endian to stdout
uint32_t get_movk_bytes(uint8_t rd, uint16_t imm16, uint8_t hw) {
    uint32_t instr = movk_instruction(rd, imm16, hw);
    return combine_bytes( instr & 0xFF,
    (instr >> 8) & 0xFF,
    (instr >> 16) & 0xFF,
    (instr >> 24) & 0xFF
    );
}

uint32_t get_svc(){
    return 0xD4000000;
}