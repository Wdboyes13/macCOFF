#include <unistd.h>
#include <sys/syscall.h>
int main() {
    int register x1 = 65;
    x1 = 65 << 16;
    int register x2 = 2;
    int register x0 = 1;
    syscall(SYS_write);
}
