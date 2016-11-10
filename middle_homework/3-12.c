#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    unsigned short perm = 0; // permission appliable
    unsigned short arg2 = 0; // argv[2]
    struct stat statbuf;

    if (argc !=3) {
        printf("usage : <filename> <octal_code>");
        return -1;
    }
    arg2 = atoi(argv[2]);
    perm |= (((arg2 / 100) % 10) << 6); // user
    perm |= (((arg2 / 10) % 10) << 3); // group
    perm |= (arg2 % 10); // others

    if (stat(argv[1],&statbuf) == -1) {
        fprintf(stderr,"addx : couldn't stat %s\n",argv[1]);
        return -1;
    }
    printf("before permission : %o\n",(0x000001FF) & statbuf.st_mode);
    if (chmod(argv[1],perm) == -1) {
        fprintf(stderr, "addx : couldn't change mode for %s\n", argv[1]);
        return -1;
    }
    printf("after permission : %o\n",perm);
    return 0;
}
