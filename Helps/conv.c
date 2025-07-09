#include <stdio.h>

int main(int argc, char* argv[]) {
    char ch = argv[1][0];  // any character
    printf("Char: %c\n", ch);
    printf("Decimal: %d\n", ch);
    printf("Hex: 0x%02X\n", ch);
    return 0;
}