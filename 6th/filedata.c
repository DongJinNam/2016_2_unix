#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "filedata.h"

static short octarray[9] = {
    0400,0200,0100,
    0040,0020,0010,
    0004,0002,0001
};

static char perms[] = "rwxrwxrwx";

int filedata(const char *pathname) {
    struct stat statbuf;
    char descrip[10];
    int j;

    if(stat(pathname,&statbuf) == -1) {
        fprintf(stderr,"Could not stat %s\n",pathname);
        exit(1);
    }

    // change the from can readable
    for (j=0; j <9; j++) {
        if (statbuf.st_mode & octarray[j])
            descrip[j] = perms[j];
        else
            descrip[j] = '-';
    }


    descrip[9] = '\0';

    // print the file data

    printf("\nFile : %s\n",pathname);
    printf("Size : %ld bytes\n",statbuf.st_size);
    printf("User-id : %d, Group-id : %d\n", statbuf.st_uid,statbuf.st_gid);
    printf("Permissions : %s\n",descrip);

    return 0;
}

