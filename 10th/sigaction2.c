#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void catchint(int);

int main(int argc, char *argv[])
{
    int i;
    struct sigaction act;
    // set the function pointer
    act.sa_handler = catchint;
    // in the catchint(), define the block signal
    sigfillset(&act.sa_mask);
    // sigint matchi with catchint(int)
    sigaction(SIGINT,&act,0);

    for (i = 0; i < 10; i++) {
        printf("Slept %d secs.\n",i+1);
        sleep(1);        
    }
    printf("Exit\n");
    return 0;
}

void catchint(int signo) {
    printf("\n");
    printf("CATCHINT : signo = %d\n",signo);
    printf("CATCHINT : returned\n");
}
