#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFSIZE 32

int main(int argc, char *argv[])
{
    int fd[2]; // one-direction communication
    pid_t pid;
    char buf[BUFSIZE];
    pipe(fd);
    pid = fork();    
    write(fd[1],"Hello World!",BUFSIZE);
    if (pid > 0) {
        while(read(fd[0],buf,BUFSIZE) > 0) {
            printf("PID : %d Message : %d %d %s", pid, fd[0], fd[1],buf);
        }
        wait(NULL);
    }
    close(fd[0]);
    close(fd[1]);
    return 0;
}
