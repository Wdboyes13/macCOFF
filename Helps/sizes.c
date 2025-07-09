#include <stdio.h>
#include "../privinc/SectionHeader.h"
#include "../privinc/FileHeader.h"
int main(){
    printf("%lu\n", sizeof(long));
    printf("%lu\n", sizeof(COFFSectionHeader));
    printf("%lu\n", sizeof(COFFHeader));
    printf("%lu\n", sizeof(COFFOpthdr));
}