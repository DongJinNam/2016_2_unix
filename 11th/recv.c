#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#define FIFO "/tmp/fifo"
#define BUFSIZE 512

int main(int argc, char *argv[])
{
    int fd;
    char buf[BUFSIZE];
    fd = open(FIFO,O_RDWR);
    if (fd < 0) return -1;
    while(read(fd,buf,BUFSIZE) > 0) {
        printf(">> %s",buf);
    }
    close(fd);
    return 0;
}
