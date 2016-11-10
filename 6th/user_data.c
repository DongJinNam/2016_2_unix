#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define PERM 0777

// this file means user_mode.c file
int main(int argc, char *argv[])
{
    char *filename = "afile";
    if (access(filename,R_OK)==-1) {
        fprintf(stderr,"User cannot read file %s\n",filename);
        exit(1);
    }
    printf("%s readable, processing \n",filename);
    if (chmod(filename, PERM) == -1)
        perror("called to chmod filed!\n");
    if (chown(filename,2000,3)==-1)
        perror("called to chown filed!\n");

    return 0;
}
