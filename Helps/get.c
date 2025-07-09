#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
void print_movz_bytes(uint8_t rd, uint16_t imm16, uint8_t hw) {
    uint32_t instr = movz_instruction(rd, imm16, hw);
    printf("movz x%d, #%u, lsl #%d → 0x%08X\nBytes (LE): %02X %02X %02X %02X\n",
        rd, imm16, hw * 16, instr,
        instr & 0xFF,
        (instr >> 8) & 0xFF,
        (instr >> 16) & 0xFF,
        (instr >> 24) & 0xFF
    );
}

int main(int argc, char* argv[]) {
        // Convert argv[1] from string to unsigned long using base 0 (auto-detect hex if prefixed with 0x)
    unsigned long val = strtoul(argv[1], NULL, 0);
    // Now cast or assign as needed (e.g., uint16_t if you want 16-bit)
    uint16_t num = (uint16_t) val;

    int rd = atoi(argv[2]);
    print_movz_bytes(rd, num, 0);  // movz x1, #0x4141
    return 0;
}