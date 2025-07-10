#include <stdio.h>
#include "../include/SectionHeader.h"
#include "../include/FileHeader.h"
int main(){
    printf("%lu\n", sizeof(long));
    printf("%lu\n", sizeof(COFFSectionHeader));
    printf("%lu\n", sizeof(COFFHeader));
    printf("%lu\n", sizeof(COFFOpthdr));
}