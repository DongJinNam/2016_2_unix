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
    //pipe system function..
    if (pid == 0) { // Child
        write(fd[1],"Hello World!",BUFSIZE);
    }
    else {
        if (pid < 0) {
            perror("Error Happend");
            exit(1);
        }
        // parent
        read(fd[0],buf,BUFSIZE);
        printf("Message : %d %d %s \n",fd[0],fd[1],buf);
        wait(NULL);
    }
    return 0;
}
