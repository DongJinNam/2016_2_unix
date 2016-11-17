#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#define FIFO "/tmp/fifo"
#define BUFSIZE 512
#define MODE 0600

int main(int argc, char *argv[])
{
    int fd;
    char buf[BUFSIZE];
    if(mkfifo(FIFO,MODE) == -1)
        return -1;
    fd = open(FIFO,O_WRONLY);
    if (fd == -1) return -2;
    while(1) {
        printf("> ");
        if(fgets(buf,BUFSIZE-1,stdin) <= 0)
            break;
        write(fd,buf,BUFSIZE);
    }
    close(fd);
    unlink(FIFO);
    return 0;
}
