#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int fileSize(const char *name) {
   int fd;
   int size;
   if ((fd = open(name,O_RDONLY)) == -1) {
       return -1;
   }
   size = lseek(fd,0,SEEK_END);
   return size;
}

// 2-8
int main(int argc, char *argv[]) {
    if (argc!=2) {
        printf("usage : <filename>\n");
        exit(1);
    }
    printf("File Size : %d\n",fileSize(argv[1]));
    return 0;
}
