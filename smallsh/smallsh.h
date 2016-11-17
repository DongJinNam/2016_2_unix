#include <stdio.h>
#include <unistd.h>

#define EOL 1 // end of line
#define ARG 2 // normal argument
#define AMPERSAND 3 // & (background : not wait, foreground : wait)
#define SEMICOLON 4 // ;
#define INTERRUPT 5
#define PIPE 6

#define MAXARG 512 // max command args
#define MAXBUF 512 // max length input line

#define BACKGROUND 0
#define FOREGROUND 1


