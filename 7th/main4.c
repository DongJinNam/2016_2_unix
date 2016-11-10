#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    struct stat buf;
    int i;

    for (i = 0; i < argc; i++)
        lstat(argv[i],&buf);

    printf("dev = %d/%d",major(buf.st_dev),minor(buf.st_dev));

    if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
        printf("(%s) rdev = %d/%d\n",(S_ISCHR(buf.st_mode)) ? "character" : "block",major(buf.st_rdev),minor(buf.st_rdev));
    }
    return 0;
}
