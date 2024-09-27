#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <stdio.h>

#define PAGE_SIZE 4096

int main() {
    int shared_fd = open("./shared_file", O_RDWR | O_CREAT, 0660);

    void* shared_mem = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);

    unsigned int* buffer = (unsigned int*) shared_mem;

    unsigned int prev_val = buffer[0];
    unsigned int cur_val = 0;

    while (1) {
        for (int i = 1; i < PAGE_SIZE / sizeof(unsigned int); i++) {
            cur_val = buffer[i];
            if (cur_val - prev_val != 1) {
                printf("\nFailure in the sequence between %u and %u\n", prev_val, cur_val);
            }
            printf("%u ", buffer[i]);
            fflush(stdout);
            prev_val = cur_val;
//            sleep(1);
        }
    }
    return 0;

}