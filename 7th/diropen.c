#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int res, depth = 0;
    DIR *dp;
    struct dirent *ent;

    if (argc !=2) {
        perror("Usage : list <path> \n");
        exit(1);
    }
    dp = opendir(argv[1]);
    if (dp == NULL)
        perror("Directory cannot be opened.\n");
    while((ent = readdir(dp)))
        printf("%10d : %s\n",ent->d_ino,ent->d_name);
    closedir(dp);
    return 0;
}
