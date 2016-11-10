#include <stdio.h>
#include <signal.h>

// void (int) structure is important.
static void fpe(int unused) {
    printf("SIGFPE caught\n");
}

int main(int argc, char *argv[])
{
    int error;
    struct sigaction act; //declaring sigaction
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_ONESHOT; // just 1, only one time
    act.sa_handler = fpe;
    sigaction(SIGFPE,&act,0); // matching the func
    error = 4/0; // if flags equal ONESHOT, fpe func only called one time.
    return 0;
}
