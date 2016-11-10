#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void listdir(char *path, int dirdepth) {
    DIR *dp;
    struct dirent *dir;
    struct stat st;

    if ((dp = opendir(path)) == NULL) return;
    chdir(path);
    while ((dir = readdir(dp)) != NULL) {
        stat(dir->d_name,&st);
        if (S_ISDIR(st.st_mode)) {
            if (strncmp(".",dir->d_name,1) == 0 || strncmp("..",dir->d_name,2) == 0) continue;
            printf("%*s%s\n",dirdepth,"",dir->d_name);
            listdir(dir->d_name,dirdepth+3);
        }
        else {
            printf("%*s%s\n",dirdepth,"",dir->d_name);
        }
    }
    chdir("..");
    closedir(dp);
}
