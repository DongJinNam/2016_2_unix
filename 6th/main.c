#include <stdio.h>
#include <stdlib.h>
#include "filedata.h"

int main(int argc, char *argv[])
{
    if (argc!=2) {
        fprintf(stderr,"Usage : execute file_input_file\n");
        exit(1);
    }
    if (filedata(argv[1]) < 0) {
        fprintf(stderr,"filedata failed\n");
        exit(1);
    }
    return 0;
}
