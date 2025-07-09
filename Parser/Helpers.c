#include <stdint.h>

uint64_t read_le64(unsigned char* buf) { // Parse a 64-Bit field in Little Endian
    return ((uint64_t)buf[0]) |
           ((uint64_t)buf[1] << 8) |
           ((uint64_t)buf[2] << 16) |
           ((uint64_t)buf[3] << 24) |
           ((uint64_t)buf[4] << 32) |
           ((uint64_t)buf[5] << 40) |
           ((uint64_t)buf[6] << 48) |
           ((uint64_t)buf[7] << 56);
}


uint16_t read_le16(unsigned char* buf){ // Parse a 16-Bit field in Little Endian
    return ((buf[0]) | ((buf[1]) << 8));
}


uint8_t read_le8(unsigned char* buf){
    return (buf[0]);
}