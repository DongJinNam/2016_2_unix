#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void cmd(int (*pipefd)[2], int count, int type, int last);

int main(int argc, char *argv[])
{
    int pipefd[3][2];
    int i, total_cmds = 3, count = 0;
    int in = 1;
    for (i =0; i < total_cmds; i++) {
        pipe(pipefd[count++]);
        cmd(pipefd,count,i,0);
    }
    cmd(pipefd,count,i,1);
    exit(EXIT_SUCCESS);
}

void cmd(int (*pipefd)[2], int count, int type, int last) {
    int child_pid, i,i2;
    if ((child_pid = fork()) == 0) {
        if (count == 1) {
            dup2(pipefd[count-1][1],1); // first command
        }
        else if (last!=1) {
            dup2(pipefd[count-2][0],0); // middle commands
            dup2(pipefd[count-1][1],1);
        }
        else if (last == 1) {
            dup2(pipefd[count-1][0],0); // last command
        }

        for (i=0; i< count; i++) {
            for (i2=0; i2<2;i2++) {
                close(pipefd[i][i2]);
            }
        }

        switch (type) {
        case 0:
            execlp("ls","ls","-al",NULL);
            break;
        case 1:
            execlp("grep","grep","main",NULL);
            break;
        case 2:
            execl("/usr/bin/wc","wc",NULL);
            break;
        case 3:
            execl("/usr/bin/wc","wc","-l",NULL);
            break;
        }
        perror("exec");
        exit(EXIT_FAILURE);
    }
    else if(child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}
