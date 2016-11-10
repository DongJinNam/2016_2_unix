#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid, ppid;
    pid = getpid();
    ppid = getppid();
    printf("PID : %d, PPID : %d\n",pid,ppid);
    return 0;
}
