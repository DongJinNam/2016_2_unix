#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int i;
    signal(SIGINT, SIG_IGN);//ignore ctrl+c
    for (i = 0; i < 10; i++) {
        sleep(1);
        printf("Slept %d secs.\n",i+1);
    }
    return 0;
}
