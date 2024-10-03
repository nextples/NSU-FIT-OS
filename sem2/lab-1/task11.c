#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int global = 5;

void *my_thread(void *arg) {
	int local = 5;
	static int local_static = 5;
	const int local_const = 5;

	 // printf("my_thread [%d %d %d %lu]: Hello from my_thread!\nlocal: [%d, %p]\nlocal_static: [%d, %p]\nlocal_const: [%d, %p]\nglobal: [%d, %p]\n\n",
			 // getpid(), getppid(), gettid(), pthread_self(), local, &local, local_static, &local_static, local_const, &local_const, global, &global);

	if (gettid() % 5 == 0) {
		local = 10;
		// local_static = 10;
		global = 10;
		// printf("VARS WERE EDITED IN %d\n", gettid());
	}
	else {
		sleep(1);
	}

	 // printf("my_thread [%d %d %d %lu]: Bye from my_thread!\nlocal: [%d, %p]\nlocal_static: [%d, %p]\nlocal_const: [%d, %p]\nglobal: [%d, %p]\n\n",
	 // getpid(), getppid(), gettid(), pthread_self(), local, &local, local_static, &local_static, local_const, &local_const, global, &global);

	sleep(300);

	return NULL;
}

int main() {
	pthread_t tids[5];
	int err;

	// printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    for (int i = 0; i < 1; i++) {
    	printf("CREATING NEW THREAD\n\n\n");
    	// sleep(1);
    	err = pthread_create(tids + i, NULL, my_thread, NULL);
		if (err) {
	    	printf("main: thread[%d] pthread_create() failed: %s\n", i, strerror(err));
			return -1;
		}

    	 printf("main [%d %d %d]: create thread %ld\n\n", getpid(), getppid(), gettid(), tids[i]);
	}

	// Дождаться выполнения потоков
	for (int i = 0; i < 5; i++) {
		pthread_join(tids[i], NULL);
	}

	return 0;
}

