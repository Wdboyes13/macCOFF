#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
void print_movk_bytes(uint8_t rd, uint16_t imm16, uint8_t hw) {
    uint32_t instr = movk_instruction(rd, imm16, hw);
    printf("movk x%d, #%u, lsl #%d → 0x%08X\nBytes (LE): %02X %02X %02X %02X\n",
        rd, imm16, hw * 16, instr,
        instr & 0xFF,
        (instr >> 8) & 0xFF,
        (instr >> 16) & 0xFF,
        (instr >> 24) & 0xFF
    );
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Usage: %s <imm16 hex> <rd> <hw>\n", argv[0]);
        return 1;
    }

    // Convert imm16 from string to unsigned long (auto detects 0x)
    unsigned long val = strtoul(argv[1], NULL, 0);
    uint16_t imm16 = (uint16_t)val;

    int rd = atoi(argv[2]);
    int hw = atoi(argv[3]);

    print_movk_bytes(rd, imm16, hw);  // Example: movk x1, #imm16, lsl #(hw*16)

    return 0;
}