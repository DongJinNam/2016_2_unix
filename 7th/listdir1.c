#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "listdir1.h"

int listdir(char *path)
{
    DIR *dp;
    struct dirent *ent;

    if ((dp = opendir(path)) == NULL) {
        fprintf(stderr,"error on open directory %s\n",path);
        return -1;
    }

    while ((ent = readdir(dp)))
        printf("%s\n",ent->d_name);
    closedir(dp);
    return 0;
}
