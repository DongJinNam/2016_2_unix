#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 512

// 2-15
int main(int argc, char *argv[]) {
    ssize_t nread;
    int filedes;
    char buf[SIZE];

    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            printf("filename : %s Content\n", argv[i]);
            if ((filedes = open(argv[i],O_RDONLY)) == -1) {
                fprintf(stderr,"file cannot opened\n");
                break;
            }
            while ((nread = read(filedes,buf,SIZE)) > 0) {
                write(1,buf,nread);
            }
        }
    }
    else {
        while ((nread = read(0,buf,SIZE)) > 0) {
            write(1,buf,nread);
        }
    }
    return 0;
}
