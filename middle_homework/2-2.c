#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define PERMS 0644

char *filename = "junkfile";

int main(int argc, char *argv[])
{
    int filedes;

    if ( (filedes = open(filename,O_RDWR | O_TRUNC, PERMS)) == -1) {
        printf("Couldn't open %s \n", filename);
        exit(1); // incorrect finish
    }

    exit(0); // correct finish

    return 0;
}
