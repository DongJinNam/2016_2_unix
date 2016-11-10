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
        printf("my parrent will be zombie in 30sec.\n");
        break;
    default:        
        sleep(30);
        break;
    }

    return 0;
}
