#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    switch(pid = fork()) {
    case -1:
        printf("error on fork()\n");
        exit(-1);
        break;
    case 0:
        sleep(30);
        break;
    default:
        printf("Child will be terminated in 30sec.\n");
        break;
    }

    return 0;
}
