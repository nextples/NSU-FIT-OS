#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include <cerrno>
#include <sys/mman.h>

#define PAGE_SIZE 4096

#define log_error() fprintf(stderr, "%s: unexpected error.\nLine: %d\nError: %s\n", __FILE__, __LINE__, strerror(errno))


void create_array_on_stack() {
    char buff[PAGE_SIZE];
    sleep(2);

    create_array_on_stack();
}

void create_array_on_heap() {
    int size = 100;

    int *array_list[size];
    for (int i = 0; i < size; i++) {
        array_list[i] = (int*) malloc(PAGE_SIZE * 256);
        sleep(1);
    }

    for (int i = 0; i < size; i++) {
        free(array_list[i]);
    }
}

void sigsegv_handler(int sig) {
    printf("received SIGSEGV\n");
    sleep(3);
    exit(1);
}

int main(int argc, char *argv[]) {
    printf("pid: %d\n", getpid());

    signal(SIGSEGV, sigsegv_handler);
    sleep(10);

//    create_array_on_stack();

//    create_array_on_heap();

//    errno = 0;
    int* ptr = (int*) mmap(NULL, 10 * PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//    log_error();

    printf("\nmapped 10 pages\n");

    sleep(5);

//    mprotect(ptr, PAGE_SIZE * 10, PROT_READ);
//    mprotect(ptr, PAGE_SIZE * 10, PROT_WRITE);

    sleep(5);

//    ptr[0] = 123;
//    int a = ptr[0];


    sleep(5);

    munmap(ptr + PAGE_SIZE * 3, PAGE_SIZE * 3);
    printf("\npages unmapped\n");

    sleep(100);
}