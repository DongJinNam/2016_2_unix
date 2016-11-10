#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int my_double_ls (const char *name) {
    struct dirent *d;
    DIR *dp;
    struct stat buf;
    char currentPath[FILENAME_MAX];

    if ((dp = opendir(name)) == NULL) return -1;
    while (d = readdir(dp)) {       
        if (strcmp(d->d_name,".") == 0 || strcmp(d->d_name,"..") == 0) continue;
        strcpy(currentPath,name);
        strcat(currentPath,"/");
        strcat(currentPath,d->d_name);

        if (stat(currentPath,&buf) == -1) continue;
        memset(currentPath,0,FILENAME_MAX);

        if (S_ISDIR(buf.st_mode))
            printf("%s*\t\n",d->d_name);
        else
            printf("%s\t\n",d->d_name);
    }
    closedir(dp);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc!=2) {
        printf("usage : <directory_path>\n");
        return -1;
    }
    if (my_double_ls(argv[1]) < 0)
        fprintf(stderr,"Error happened!\n");
    return 0;
}
