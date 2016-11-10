#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

char *workfile = "junkfile";

int main(int argc, char *argv[])
{
    int filedes;

    if ( (filedes = open(workfile,O_RDONLY)) == -1) {
        printf("Couldn't open %s \n", workfile);
        exit(1); // incorrect finish
    }

    exit(0); // correct finish

    return 0;
}
