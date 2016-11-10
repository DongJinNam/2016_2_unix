#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
        sleep(10);
    else if (pid > 0) {
        printf("Child pid : %d\n",pid);
        pid = wait(NULL);
        printf("Child waited : %d\n",pid);
    }
    return 0;
}
