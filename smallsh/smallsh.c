#include "smallsh.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

//static variables in smallsh
static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr = inpbuf, *tok = tokbuf;
static char special[] = {' ','\t','&',';','\n','\0'};
int intr_p = 0;
char *prompt = "Command>"; // prompt
int fg_pid = 0;

//all functions that used in smallsh
int userin(char *);
void procline(void);
int inarg(char);
int gettok(char **); // get token type
int runcommand(char **, int); // command processing
void handle_int(int); // sigaction handler

// main function
int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT,&sa,0);
    sigaction(SIGQUIT,&sa,0);

    while(userin(prompt) != EOF) {
        procline();
    }
    return 0;
}

// function that primitives come from user typing to smallsh.
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

// function that decide type of primitives that user inputs.
void procline(void) {
    char *arg[MAXBUF+1]; // pointer array for runcommand
    int toktype; // type of token in command
    int narg;// number of arguments so far
    int type;// FOREGROUND or BACKGROUND

    narg = 0; // reset integer flag

    for (;;) {
        // take action according token type
        switch (toktype = gettok(&arg[narg])) {
        case ARG:
            if (narg < MAXARG) narg++;
            break;
        case EOL:
        case AMPERSAND:
        case SEMICOLON:
            if (toktype == AMPERSAND)
                type = BACKGROUND;
            else
                type = FOREGROUND;

            if (narg != 0) {
                arg[narg] = NULL;
                runcommand(arg,type);
            }
            if (toktype == EOL) return;
            narg = 0;
            break;
        default:
            break;
        }
    }
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
        if (getcwd(path,MAXBUF) != NULL)
            printf("change directory is : %s\n",path);
        else
            perror("getcwd() error");
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
    int c;
    if(!fg_pid) {
        // ctrl+c hit at shell command
        printf("\n");
        exit(1);
    }
    if (intr_p) {
        printf("\ngot it, signalling\n");
        //if child process existed, kill the child process
        kill(fg_pid,SIGTERM);
        fg_pid = 0;
    }
    else {
        printf("\nignoring, type ^c again to interrupt\n");
        intr_p = 1;
    }
}
