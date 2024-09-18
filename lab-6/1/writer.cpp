#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

int main() {
    int shared_fd = open("./shared_file", O_RDWR | O_CREAT, 0660);
    ftruncate(shared_fd, 0);
    ftruncate(shared_fd, PAGE_SIZE);

    void* shared_mem = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);

    unsigned int* buffer = (unsigned int*) shared_mem;

    unsigned int val = 0;
    while (1) {
        for (int i = 0; i < PAGE_SIZE / sizeof(unsigned int); i++) {
            buffer[i] = val++;
//            sleep(1);
        }
    }
    return 0;

}