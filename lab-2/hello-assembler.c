#include <string.h>
#include <asm/unistd_64.h>

void print(char* string, int file_descriptor) {
    size_t length = strlen(string);
    asm (
            "mov %0, %%rax\n"
            "mov %1, %%rdi\n"
            "mov %2, %%rsi\n"
            "mov %3, %%rdx\n"
            "syscall\n"
            :
            : "g"(__NR_write), "g"(file_descriptor), "g"(string), "g"(length)
            : "%rax", "%rdi", "%rsi", "%rdx"
            );
}

int main() {
    print("Hello world!\n", 1);
    return 0;
}
