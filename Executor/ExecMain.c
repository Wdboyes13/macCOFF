#include <stdint.h>
#include "../privinc/Calls.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define REG_COUNT 32

uint64_t regs[REG_COUNT] = {0};

void ExecMain(unsigned char* Exec, long s_size, long file_offset, uint8_t* DataSect, long DataSize){ // Executor for .text (Executable Data)
    long pc = 0;
    printf("[DEBUG] pc starts at: 0x%lx\n", pc);
    while (pc < s_size){
        uint32_t instr = (Exec[pc]) | // Fetch Instruction - LE 4B
                         (Exec[pc + 1] << 8) |
                         (Exec[pc + 2] << 16) |
                         (Exec[pc + 3] << 24);

        // === MOVZ CHECK ===
        // Check for opcode bits: top 9 bits == 0b110100101 (MOVZ)
        if ((instr & 0xFFC00000) == 0xD2800000) {
            uint8_t rd = instr & 0x1F;
            uint8_t hw = (instr >> 21) & 0x3;
            uint16_t imm16 = (instr >> 5) & 0xFFFF;

            if (rd < REG_COUNT) {
                regs[rd] = ((uint64_t)imm16) << (hw * 16);
                printf("movz x%d, #%u (=> 0x%llu)\n", rd, imm16, regs[rd]);
            } else {
                fprintf(stderr, "Invalid register x%d\n", rd);
            }
        }

        // === MOVK CHECK ===
        // Check for opcode bits: top 9 bits == 0b110100101 (MOVZ)
        else if ((instr & 0xFF800000) == 0xF2800000) {
            uint8_t rd = instr & 0x1F;
            uint8_t hw = (instr >> 21) & 0x3; // Halfword offset
            uint16_t imm16 = (instr >> 5) & 0xFFFF;

            if (rd < REG_COUNT) {
                // Clear the 16-bit chunk we're about to overwrite
                uint64_t mask = ~(0xFFFFULL << (hw * 16));
                regs[rd] = (regs[rd] & mask) | ((uint64_t)imm16 << (hw * 16));
                printf("movk x%d, #%u, lsl #%d (=> 0x%llx)\n", rd, imm16, hw * 16, regs[rd]);
            }
        }

        // === SVC ===
        else if ((instr & 0xFFFF0000) == 0xD4000000) {
            uint16_t svc_num = instr & 0xFFFF;
            if (svc_num == 0x0) {
                uint64_t syscall = regs[16];
                printf("svc #0 → syscall %llu\n", syscall);

                switch (syscall) {
                    case COFF_EXIT:
                        printf("[ VM Exit ] exit(%llu)\n", regs[0]);
                        exit(regs[0]);
                    case COFF_WRITE:
                        printf("[ VM Write ] write(%llu, %llu, %llu)\n", regs[0], regs[1], regs[2]);
                        write(regs[0], &regs[1], regs[2]);
                        printf("\n");
                        break;
                    case COFF_WRITE_DATA: {
                        uint64_t fd = regs[0];
                        uint64_t DataOff = regs[1];
                        uint64_t WDataSize = regs[2];
                        
                        // Make sure offset + size doesn't overflow your .data buffer!
                        if (DataOff + WDataSize > DataSize) {
                            fprintf(stderr, "Error: write out of .data bounds!\n");
                            return; // Or handle error accordingly
                        }

                        // Pointer into your .data buffer at offset DataOff
                        uint8_t* write_ptr = &DataSect[DataOff];
                        
                        printf("[ VM Write .data ] write(fd=%llu, data_offset=%llu, size=%llu)\n", fd, DataOff, WDataSize);
                        
                        ssize_t written = write(fd, write_ptr, WDataSize);
                        if (written < 0) {
                            perror("write failed");
                        } else {
                            printf("[ VM Write .data ] wrote %zd bytes\n", written);
                        }
                        break;
                    }
                    default:
                        fprintf(stderr, "Unknown syscall: %llu\n", syscall);
                        return;
                }
            } else {
                fprintf(stderr, "Unhandled svc #%u\n", svc_num);
            }
        }

        else {
            fprintf(stderr, "Unknown instruction @0x%lx (file offset 0x%lx): 0x%08x\n",
                    pc, file_offset + pc, instr);
        }

        pc += 4;
    }
    return;
}   