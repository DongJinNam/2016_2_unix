#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    struct stat buf1,buf2;
    char name[512];

    if (argc != 3) {
        printf("usage : <symbolic link> <origin link>\n");
        exit(-1);
    }
    if (lstat(argv[1],&buf1) == -1) {
        perror("lstat() error\n");
    }
    else {
        printf("st_inode1 : %d\n",buf1.st_ino);
    }

    if (stat(argv[2],&buf2) == -1) {
        perror("stat() error\n");
    }
    else {
        printf("st_inode2 : %d\n",buf2.st_ino);
    }


    if(readlink(argv[1],name,512)==-1) {
        perror("readlink() error\n");
    }
    else {
        printf("read link content name : %s\n",name);
    }


    return 0;
}
