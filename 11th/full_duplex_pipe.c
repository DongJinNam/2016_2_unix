#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFSIZE 32

int main(int argc, char *argv[])
{
    int fd[2][2];
    pid_t pid;
    char buf[BUFSIZE];

    assert(pipe(fd[0]) == 0);
    assert(pipe(fd[1]) == 0);

    pid = fork();

    if (pid == 0) {
        close(fd[0][1]); close(fd[1][0]);
        write(fd[1][1], "Hello Parent!", BUFSIZE);
        read(fd[0][0],buf,BUFSIZE);
        printf("Child Message : %s\n",buf);
    }
    else if (pid > 0){
        close(fd[0][0]);close(fd[1][1]);
        write(fd[0][1],"Hello Child!",BUFSIZE);
        sleep(5); // diffrentiate parent and child
        read(fd[1][0],buf,BUFSIZE);
        printf("Parent Message : %s\n",buf);
    }
    return 0;
}
