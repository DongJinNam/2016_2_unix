#include <stdio.h>
#include <unistd.h>

#define EOL 1 // 라인의 끝
#define ARG 2 // 문자열 argument
#define AMPERSAND 3 // & (background : 부모가 자식 프로세스를 기다리지 않음, foreground : 부모가 자식 프로세스를 기다림)
#define SEMICOLON 4 // 세미콜론 기호
#define INTERRUPT 5 // 인터럽트 기호
#define PIPE 6 // pipe 기호

#define MAXARG 512 // 최대 argument 개수
#define MAXBUF 512 // 입력 라인의 최대 길이

#define BACKGROUND 0
#define FOREGROUND 1

// smallsh에 선언된 모든 함수들
int userin(char *); // 사용자로부터 명령어를 입력받는 함수
void procline(void); // 사용자로부터 입력받은 명령어들의 type을 결정하는 함수
int inarg(char); // 문자열이 일반 문열인지 특수 문자열인지 판별하는 함수
int gettok(char **); // 매개변수 문자열의 token type을 리턴하는 함수
int runcommand(char **, int); // smallsh에 입력된 명령어 처리 함수
int pipecommand(char **, int); // 파이프 처리 함수
void handle_int(int); // signal handler (SIGINT, SIGQUIT)


