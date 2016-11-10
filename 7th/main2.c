
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc > 3) {
        fprintf(stderr, "Usage : listdir [target]\n");
        return -1;
    }
    if (argc == 1) {
        listdir(".",0);
    }
    if (argc == 2) {
        listdir(argv[1],0);
    }
    return 0;
}
