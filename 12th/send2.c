#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define FIFO "/tmp/fifo"
#define BUFSIZE 512
#define MODE 0600

void handler_int(int);
int fd;

int main(int argc, char *argv[])
{
    char buffer[BUFSIZE];
    struct sigaction act;

    act.sa_handler = handler_int;
    act.sa_flags = SA_ONESHOT;
    sigaction(SIGINT,&act,NULL);
    sigaction(SIGQUIT,&act,NULL);
    sigaction(SIGTSTP,&act,NULL);

    if (mkfifo(FIFO,MODE) == -1) {
        printf("error on mkfifo\n");
        return -1;
    }
    while((fd = open(FIFO, O_WRONLY | O_NONBLOCK)) < 0) {
        if (errno != ENXIO) return -2;
        printf("no process is reading. waiting 1 sec\n");
        sleep(1);
    }
    while(1) {
        printf("> ");
        if(fgets(buffer,BUFSIZE-1,stdin)<=0) break;
        write(fd,buffer,BUFSIZE);
    }
    close(fd);
    unlink(FIFO);
    return 0;
}

void handler_int(int s) {
    close(fd);
    unlink(FIFO);
    exit(0);
}
