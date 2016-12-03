#include "smallsh.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

//smallsh에 선언된 정적변수들
static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr = inpbuf, *tok = tokbuf;
static char special[] = {' ','\t','&',';','\n','\0'};
char *prompt = "Command>"; // 명령문
int fg_pid = 0;
static int pipecnt = 0; //사용된 pipe의 개수

// smallsh에 선언된 모든 함수들
int userin(char *);
void procline(void);
int inarg(char);
int gettok(char **); // 매개변수 문자열의 token type을 리턴하는 함수
int runcommand(char **, int); // smallsh에 입력된 명령어 처리 함수
int pipecommand(char ***, int); // 파이프 처리 함수
void handle_int(int); // signal catch 함수

// 메인 함수
int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = handle_int;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT,&sa,0);
    sigaction(SIGQUIT,&sa,0);

    while(userin(prompt) != EOF) {
        procline();
    }
    return 0;
}

// 사용자로부터 명령어를 입력받는 함수
int userin(char *p) {
    int c,count;
    ptr = inpbuf;
    tok = tokbuf;

    printf("%s",p);
    count = 0;
    while(1) {
        if ((c = getchar()) == EOF)
            return EOF;
        if (count < MAXBUF)
            inpbuf[count++] = c;
        if (c == '\n' && count < MAXBUF) {
            inpbuf[count] = '\0';
            return count;
        }
        if (c == '\n') {
            printf("smallsh : input line too long\n");
            count = 0;
            printf("%s",p);
        }
    }
}

// 사용자로부터 입력받은 명령어들의 type을 결정하는 함수
void procline(void) {
    char *arg[MAXBUF+1]; // runcommand 혹은 pipecommand에 사용될 함수
    char *arg2[MAXBUF+1]; // pointer array for pipecommand
    char *arg3[MAXBUF+1]; // pointer array for pipecommand
    char *arg4[MAXBUF+1]; // pointer array for pipecommand
    char **cline[] = {arg,arg2,arg3,arg4,NULL};
    int toktype; // 입력받은 token의 type
    int narg = 0; // argument 매개변수의 개수
    int type;// FOREGROUND or BACKGROUND
    int i, index;

    pipecnt = 0; // if pipe used. pipe > 0

    for (;;) {
        // take action according token type        
        char **temp = *(cline + pipecnt);
        toktype = gettok(&temp[narg]);

        switch (toktype) {
        case ARG:
            if (narg < MAXARG)
                narg++;
            break;
        case EOL:
        case AMPERSAND:
        case SEMICOLON:
            if (toktype == AMPERSAND)
                type = BACKGROUND;
            else
                type = FOREGROUND;

            if (pipecnt) {
                if (narg != 0) {
                    temp[narg] = NULL;
                    pipecommand(cline,type);
                }
            }
            else {
                if (narg != 0) {
                    temp[narg] = NULL;
                    runcommand(arg,type);
                }
            }

            if (toktype == EOL) return;
            narg =0;
            break;
        case PIPE:
//            narg1 = narg;
//            narg = 0;
            temp[narg] = NULL;
            pipecnt++;
            narg = 0;
            break;
        default:
            break;
        }
    }
    pipecnt = 0; // 파이프 초기화
}

// function that string is special or not special.
int inarg(char c) {
    char *wrk;
    for (wrk = special; *wrk; wrk++) {
        if (c == *wrk) return 0;
    }
    return 1;
}

// function that tokenizing input primitives
int gettok(char **outptr) {
    int type;
    *outptr = tok;

    for (;*ptr == ' ' || *ptr == '\t'; ptr++) {
    }
    *tok++ = *ptr;
    switch (*ptr++) {
    case '\n':
        type = EOL;
        break;
    case '&':
        type = AMPERSAND;
        break;
    case ';':
        type = SEMICOLON;
        break;
    case '|': // pipe added
        type = PIPE;
        break;
    default:
        type = ARG;
        while(inarg(*ptr)) {
            *tok++ = *ptr++;
        }
        break;
    }
    *tok++ = '\0';
    return type;
}

//function that processing input primitives
int runcommand(char **cline, int where) {
    pid_t pid;
    int status;
    char path[MAXBUF]; // if cd command included, current directory path

    // ignore signal
    struct sigaction sa_ign, sa_conf;
    sa_ign.sa_handler = SIG_IGN;
    sigemptyset(&sa_ign.sa_mask);
    sa_ign.sa_flags = SA_RESTART;

    sa_conf.sa_handler = handle_int;
    sigemptyset(&sa_conf.sa_mask);
    sa_conf.sa_flags = SA_RESTART;

    // first change directory command will be processed in parent process
    if (strcmp(*cline,"cd") == 0) {
        chdir(*(cline+1));
        fg_pid = getpid(); // pid update
        return -1;
    }

    switch(pid = fork()) {
        case -1:
            perror("smallsh");
            break;
        case 0:
            sigaction(SIGINT,&sa_ign,0);
            sigaction(SIGQUIT,&sa_ign,0);
            execvp(*cline,cline);
            perror(*cline);
            exit(127);
        default:
            fg_pid = pid;
            break;
    }
    // BACKGROUND means parent not wait child process
    if (where == BACKGROUND) {
        fg_pid = 0;
        printf("[Process id %d]\n",pid);
        return 0;
    }

    sigaction(SIGINT,&sa_conf,0);
    sigaction(SIGQUIT,&sa_conf,0);

    // wait child process and return value
    return waitpid(pid,&status,0) == -1 ? -1 : status;
}

//sigaction handler
//parameter int not used, but need to exist.s
void handle_int(int s) {
    switch(s) {
    case SIGINT:
        printf("\nSIGINT Received.\n");
        break;
    case SIGQUIT:
        printf("\nSIGQUIT Received.\n");
        break;
    }
    if(!fg_pid) {
        // ctrl+c and ctrl+\ signal handling
        exit(1);
    }
    else {
        //if child process existed, kill the child process
        kill(fg_pid,SIGTERM);
        fg_pid = 0;
    }
}

int pipecommand(char ***cline, int where) {
    // if sentense have only one pipe identifier..
    //pid_t pid, pid2;
    pid_t pid[4] = {0};
    int status;
    int fds[2];
    int narg = 0,i=0;
    int fd_in =0;
    // ignore signal in pipe processing
    struct sigaction sa_ign, sa_conf;
    sa_ign.sa_handler = SIG_IGN;
    sigemptyset(&sa_ign.sa_mask);
    sa_ign.sa_flags = SA_RESTART;

    sa_conf.sa_handler = handle_int;
    sigemptyset(&sa_conf.sa_mask);
    sa_conf.sa_flags = SA_RESTART;

    sigaction(SIGINT,&sa_ign,0);
    sigaction(SIGQUIT,&sa_ign,0);

    for (i=0;i<pipecnt+1;i++) {
        pipe(fds);
        if ((pid[i] = fork())==-1) {
            exit(EXIT_FAILURE);
        }
        else if(pid[i] == 0) {
            dup2(fd_in,0);
            if (i!=pipecnt)
                dup2(fds[1],1);
            close(fds[0]);
            execvp((*cline)[0],*cline);
            exit(EXIT_FAILURE);
        }
        else {
            if (where != BACKGROUND) {
                if(waitpid(pid[i],&status,0)==-1) return -1;
            }
            else {
                if (i == pipecnt) {
                    fg_pid = 0;
                    printf("[Process id %d]\n",pid[pipecnt]);
                    return 0;
                }
                else {
                    if(waitpid(pid[i],&status,0)==-1) return -1;
                }
            }
            close(fds[1]);
            fd_in = fds[0];
            cline++;
        }
    }

    close(fds[0]);close(fds[1]);

    sigaction(SIGINT,&sa_conf,0);
    sigaction(SIGQUIT,&sa_conf,0);
    return status;
}
