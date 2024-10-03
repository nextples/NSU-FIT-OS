#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* long_work(void* arg) {
    printf("New thread have started...\n");

    for (int i = 0; i < 10; ++i) {
        printf("Thread is working: step %d\n", i);
        sleep(1);
    }
    printf("Thread is terminated\n");
    return NULL;
}

int main() {
    pthread_t thread;

    if (pthread_create(&thread, NULL, long_work, NULL) != 0) {
        perror("Fail to create thread!\n");
        return 1;
    }

    printf("Main thread is terminated!\n");
    pthread_exit(NULL);
}
