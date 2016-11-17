#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFSIZE 512

int main(int argc, char *argv[])
{
    FILE *pipe;
    char buf[BUFSIZE];
    pipe = popen("ls -l","r");
    if (pipe == NULL) {
        perror("popen() ");
        return -1;
    }
    while(fgets(buf,BUFSIZE,pipe))
        printf("%s",buf);
    pclose(pipe);
    return 0;
}
