#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    printf("Calling fork()\n");
    pid = fork();
    if(pid > 0)
        // chiid pid, current pid, parent pid
        printf("by parent : %d, %d, %d\n",pid,getpid(),getppid());
    else if(pid == 0)
        // 0, current pid, parent pid
        printf("by child : %d, %d, %d\n",pid,getpid(),getppid());
    else
        printf("error on fork()\n");

    return 0;
}
