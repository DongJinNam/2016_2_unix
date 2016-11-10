#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 512

int main(int argc, char *argv[])
{
    int fd, fd2;
    char input[10] = "input_arr_";
    char output[BUFSIZE];
    ssize_t nread;

    if (argc != 2) {
        printf("usage : <new_file_name>\n");
        exit(1);
    }

    /* 1. creat + write + read */
    if((fd = creat(argv[1], 0644)) == -1) {
        printf("fd : %d - creat() error\n", fd);
        exit(1);
    }
    else printf("fd : %d - creat() success\n", fd);

    if((write(fd,input, BUFSIZE) == -1)) {
        printf("fd %d - write() error\n",fd);
    }
    else printf("fd %d - write() success\n", fd);

    // error happened
    if((nread = read(fd,output,BUFSIZE)) == -1) {
        printf("fd %d - read() error\n", fd);
    }
    else printf("fd %d - read() success\n",fd, output);

    /* 2. open + write + read */
    if((fd2 = open(argv[1],O_RDWR | O_CREAT)) == -1) {
        printf("fd %d - open() error\n", fd2);
        exit(1);
    }
    else printf("fd %d - open() success\n", fd2);

    // error happened
    if((nread = read(fd,output,BUFSIZE)) == -1) {
        printf("fd %d - read() error\n", fd);
    }
    else printf("fd %d - read() success\n",fd, output);

    if((nread = read(fd2,output,BUFSIZE)) == -1) {
        printf("fd %d - read() error\n", fd2);
    }
    else printf("fd %d - read() success\n",fd2, output);

    close(fd);
    return 0;
}
