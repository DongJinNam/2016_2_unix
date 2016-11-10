#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 512

int main(int argc, char *argv[])
{
    char buffer[BUFSIZE];
    int filedes;
    ssize_t nread;
    long total = 0;

    // another file is only used for read
    if ((filedes = open("anotherfile",O_RDONLY))==-1) {
        perror("error in opening anotherfile\n");
        return -1;
    }

    // repeat until EOF
    while((nread = read(filedes,buffer,BUFSIZE)) > 0)
        total += nread;

    printf("total chars in anotherfile : %ld\n",total);
    return 0;
}
