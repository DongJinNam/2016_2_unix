#include "smallsh.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

//smallsh에 선언된 정적변수들
//inpbuf : 유저로 부터 입력받는 문자열을 저장, tokbuf : 입력받은 문자열을 token 분할시 사용
static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr = inpbuf, *tok = tokbuf;
static char special[] = {' ','\t','&',';','\n','\0','|'}; // 특수 문자열 모음
char *prompt = "Command>"; // 명령문
int fg_pid = 0; // foreground process id
static int fd_in = 0; //pipecommand에서 사용할 입력 파일 디스크립터
static int pipecnt = 0; //사용된 pipe의 개수
static int pipefds[2]; //pipecommand에서 사용될 pipe

// 메인 함수
int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = handle_int;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    //기본적으로 signal Handler 처리 시에 signal을 catch 하도록 한다. (SIGINT, SIGQUIT에 대해)
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
    char *arg[MAXBUF+1]; // runcommand 혹은 pipecommand에 사용될 문자열
    int toktype; // 입력받은 token의 type
    int narg = 0; // argument 매개변수의 개수
    int type;// FOREGROUND or BACKGROUND

    pipecnt = 0; // 파이프 기호가 입력된 경우 0보다 크다, 그렇지 않은 경우 0

    for (;;) {
        // 입력 문자열 버퍼에 따라서 token type을 설정하고 token 문자열을 얻어온다.
        toktype = gettok(&arg[narg]);

        switch (toktype) {
        case ARG:
            if (narg < MAXARG)
                narg++;
            break;
        case EOL: // End of Line
        case AMPERSAND: // &
        case SEMICOLON: // ;
            if (toktype == AMPERSAND)
                type = BACKGROUND;
            else
                type = FOREGROUND;

            if (pipecnt) { // pipecnt > 0 : 파이프 기호가 사용되었다는 의미
                if (narg != 0) {
                    arg[narg] = NULL;
                    pipecommand(arg,1);
                }
            }
            else { // 파이프를 사용하지 않은 경우
                if (narg != 0) {
                    arg[narg] = NULL;
                    runcommand(arg,type);
                }
            }

            if (toktype == EOL) return;
            narg = 0;
            pipecnt = 0; // 파이프 관련 변수들 초기화
            fd_in = 0; // 파이프 관련 변수들 초기화
            break;
        case PIPE:
            if (narg != 0) {
                arg[narg] = NULL;
                pipecommand(arg,0);
            }
            pipecnt++; // 파이프 기호를 발견할 수록, 파이프 개수를 증가시키도록 한다.
            narg = 0;
            break;
        default:
            break;
        }
    }
    pipecnt = 0; // 파이프 개수 초기화
}

// 문자열이 일반 문열인지 특수 문자열인지 판별하는 함수
int inarg(char c) {
    char *wrk;
    for (wrk = special; *wrk; wrk++) {
        if (c == *wrk) return 0;
    }
    return 1;
}

// 매개변수 문자열의 token type을 리턴하는 함수
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
    case '|': // 파이프 기호를 추가하였다.
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

// smallsh에 입력된 명령어 처리 함수
int runcommand(char **cline, int where) {
    pid_t pid; // 프로세스 id
    int status;

    //sa_ign : 시그널을 무시하는 sigaction, sa_conf : 시그널을 handler 함수로 처리하는 sigaction
    struct sigaction sa_ign, sa_conf;
    sa_ign.sa_handler = SIG_IGN;
    sigemptyset(&sa_ign.sa_mask);
    sa_ign.sa_flags = SA_RESTART;

    sa_conf.sa_handler = handle_int;
    sigemptyset(&sa_conf.sa_mask);
    sa_conf.sa_flags = SA_RESTART;

    // cd 명령어가 들어올 시에는 부모 프로세스에서 처리하도록 한다.
    if (strcmp(*cline,"cd") == 0) {
        chdir(*(cline+1));
        return -1;
    }

    // cd 명령어 특수 경우를 제외한 나머지 경우는 fork로 자식 프로세스를 만들어서 처리한다.
    switch(pid = fork()) {
        case -1:
            perror("smallsh");
            break;
        case 0:
            // 자식 프로세스를 실행 중일 때는 SIGINT, SIGQUIT Signal을 무시하도록 한다.
            sigaction(SIGINT,&sa_ign,0);
            sigaction(SIGQUIT,&sa_ign,0);
            execvp(*cline,cline);
            perror(*cline);
            exit(127);
        default:
            fg_pid = pid;
            break;
    }
    // BACKGROUND 기호는 부모가 자식 프로세스를 기다리지 않는다.
    if (where == BACKGROUND) {
        fg_pid = 0;
        printf("[Process id %d]\n",pid);
        return 0;
    }

    // 자식 프로세스를 만들고 회수하고 나서는 SIGINT, SIGQUIT signal을 handler로 처리하도록 한다.
    sigaction(SIGINT,&sa_conf,0);
    sigaction(SIGQUIT,&sa_conf,0);

    // wait child process and return value
    return waitpid(pid,&status,0) == -1 ? -1 : status;
}

//signal handler 함수
//파라미터는 단순히 시그널 타입 전달용이]다.
void handle_int(int s) {
    switch(s) {
    case SIGINT:
        // ctrl + c
        printf("\nSIGINT Received.\n");
        break;
    case SIGQUIT:
        // ctrl + \ //
        printf("\nSIGQUIT Received.\n");
        break;
    }
    if(!fg_pid) {
        exit(1);
    }
    else {
        // child process 가 존재하는 경우에는 자식 프로세스를 kill 하도록 한다.
        kill(fg_pid,SIGTERM);
        fg_pid = 0;
    }
}

// 파이프 명령어가 사용된 경우, 처리하는 함수
int pipecommand(char **cline, int last) {
    pid_t pid;
    int status; // 자식 프로세스가 정상적으로 종료됬는지 확인하는 변수

    //sa_ign : 시그널을 무시하는 sigaction, sa_conf : 시그널을 handler 함수로 처리하는 sigaction
    struct sigaction sa_ign, sa_conf;
    sa_ign.sa_handler = SIG_IGN;
    sigemptyset(&sa_ign.sa_mask);
    sa_ign.sa_flags = SA_RESTART;

    sa_conf.sa_handler = handle_int;
    sigemptyset(&sa_conf.sa_mask);
    sa_conf.sa_flags = SA_RESTART;

    sigaction(SIGINT,&sa_ign,0);
    sigaction(SIGQUIT,&sa_ign,0);

    // 파이프 설정
    pipe(pipefds);
    switch((pid = fork())) {
    case -1: // fork 에러 발생 시
        exit(EXIT_FAILURE);
    case 0: // 자식 프로세스 내에서
        dup2(fd_in,0); // stdin filedescriptor 복사
        if (last == 0) // 마지막이 아닌 경우
            dup2(pipefds[1],1); // stdout filedescriptor 복사
        close(pipefds[0]);
        execvp(*cline,cline); // exec 실행
        exit(EXIT_FAILURE);
    default:
        if(waitpid(pid,&status,0)==-1) return -1; // 자식 프로세스를 기다림
        close(pipefds[1]);
        fd_in = pipefds[0]; // 파이프 입력 부분 file descriptor를 fd_in에 저장한다.
        break;
    }
    if (last) {
        // 마지막인 경우 파이프 사용을 종료한다.
        close(pipefds[0]);close(pipefds[1]);
    }
    //자식 프로세스의 기능을 모두 사용했으므로, sigaction도 원상태로 돌린다.
    sigaction(SIGINT,&sa_conf,0);
    sigaction(SIGQUIT,&sa_conf,0);
    return status;
}
