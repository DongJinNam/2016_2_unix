#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thr_fn1(void *arg);
void *thr_fn2(void *arg);

int main(int argc, char *argv[])
{
    int err;
    pthread_t tid1, tid2;
    void *tret;

    if((err = pthread_create(&tid1,NULL,thr_fn1,NULL)) != 0) {
        perror("can't create thread 1\n");
        exit(1);
    }
    if((err = pthread_create(&tid2,NULL,thr_fn2,NULL)) != 0) {
        perror("can't create thread 2\n");
        exit(1);
    }
    if((err = pthread_join(tid1,&tret)) != 0) {
        perror("can't join with thread 1\n");
        exit(1);
    }
    printf("thread 1 exit code : %d \n", (int) tret);

    if((err = pthread_join(tid2,&tret)) != 0) {
        perror("can't join with thread 2\n");
        exit(1);
    }
    printf("thread 2 exit code : %d \n", (int) tret);

    exit(EXIT_SUCCESS);
}

void *thr_fn1(void *arg) {
    printf("thread 1 returning\n");
    return ((void *)1);
}

void *thr_fn2(void *arg) {
    printf("thread 2 exciting\n");
    return ((void *)2);
}
