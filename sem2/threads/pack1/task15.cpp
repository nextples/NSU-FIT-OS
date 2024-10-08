#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *block_signals_routine(void *arg) {
    sigset_t signal_set;
    sigfillset(&signal_set);
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);

    printf("block_signals_routine [%d %d %d %ld]: Blocked all signals\n",
        getpid(), getppid(), gettid(), pthread_self());

    sleep(10);

    return NULL;
}

void sigint_handler(int signo) {
    printf("sigint_handler [%d %d %d]: Received SIGINT\n",
        getpid(), getppid(), gettid());
}

void *sigint_handling_routine(void *arg) {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sa.sa_mask = set;
    sigaction(SIGINT, &sa, NULL);

    printf("sigint_handling_routine [%d %d %d %ld]: Set up SIGINT handler\n",
        getpid(), getppid(), gettid(), pthread_self());

    sleep(10);

    return NULL;
}


void *wait_sigquit_routine(void *arg) {
    int sig;
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGQUIT);

    printf("wait_sigquit_routine [%d %d %d %ld]: Waiting for SIGQUIT\n",
        getpid(), getppid(), gettid(), pthread_self());

    sigwait(&signal_set, &sig);

    printf("wait_sigquit_routine [%d %d %d %ld]: Received SIGQUIT\n",
        getpid(), getppid(), gettid(), pthread_self());

    pthread_exit(NULL);
}

int main() {
    pthread_t tid1, tid2, tid3;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    pthread_create(&tid1, NULL, block_signals_routine, NULL);

    pthread_create(&tid2, NULL, sigint_handling_routine, NULL);

    pthread_create(&tid3, NULL, wait_sigquit_routine, NULL);

    pthread_kill(tid1, SIGINT);
    pthread_kill(tid1, SIGSEGV);
    pthread_kill(tid1, SIGQUIT);
    printf("main [%d %d %d]: block_signals_routine blocks signals\n", getpid(), getppid(), gettid());

    sleep(3);

    pthread_kill(tid2, SIGINT);

    sleep(3);

    pthread_kill(tid3, SIGQUIT);

    pthread_join(tid1, NULL);
    printf("main [%d %d %d]: Join block_signals_routine\n", getpid(), getppid(), gettid());

    pthread_join(tid2, NULL);
    printf("main [%d %d %d]: Join sigint_handling_routine\n", getpid(), getppid(), gettid());

    pthread_join(tid3, NULL);
    printf("main [%d %d %d]: Join wait_sigquit_routine\n", getpid(), getppid(), gettid());

    return 0;
}