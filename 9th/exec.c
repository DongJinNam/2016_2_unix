#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    switch(pid = fork()) {
    case -1:
        printf("error on fork()\n");
        exit(-1);
        break;
    case 0:
        execl("/bin/ls","ls","-l",(char *)0);
        printf("exec failed");
        break;
    default:        
        wait((int *)0);
        printf("Is Completed.\n");
        break;
    }
    return 0;
}
