#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int ntimes=0;

int main(int argc, char *argv[])
{
    pid_t pid, ppid;
    static struct sigaction pact, cact;
    void p_action(int);
    void c_action(int);

    // signal processing for parent process
    pact.sa_handler = p_action;
    sigaction(SIGUSR1,&pact,0);
    switch (pid = fork()) {
    case -1:
        perror("Synchro");
        break;
    case 0: // child
        cact.sa_handler = c_action;
        sigaction(SIGUSR1,&cact,0);
        ppid = getppid();
        for (;;) {
            sleep(1);
            kill(ppsid,SIGUSR1);
            pause();
            //raise(SIGUSR1);
        }
        break;
    default: // parent
        for (;;) {
            pause();
            sleep(1);
            kill(pid,SIGUSR1);
        }
        break;
    }
    return 0;
}

void p_action(int sig) {
    printf("Parent caught signal #%d\n",++ntimes);
}

void c_action(int sig) {
    printf("Child caught signal #%d\n",++ntimes);
}

