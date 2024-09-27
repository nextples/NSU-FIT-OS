#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("pid: %d\n", getpid());
    sleep(1);

//    char* buff = (char*) malloc(1024*1024*1024);
//    buff[0] = '1';

//    for (int i = 0; i < 5; i++) {
//        fork();
//        printf("\n%c", buff[0]);
//    }
//    sleep(1000);
    execv(argv[0], argv);
    printf("Hello world");
}