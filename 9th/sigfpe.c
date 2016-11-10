#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void fpe(int unused) {
    printf("SIGFPE caught\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    int error;
    signal(SIGFPE,fpe);
    error = 4/0;
    return 0;
}
