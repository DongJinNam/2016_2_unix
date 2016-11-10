#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

unsigned short lsock(const char *code) {
    // file's permission code to octal code
    // ex. code = "rw-r--r--" -> value = 0644
    unsigned short value = 0;
    char ch;
    int i, len = strlen(code);

    for (i = 0; i < len; i++) {
        ch = code[i];
        switch(ch) {
        case 'r': // read
        case 'R':
        case 'w': // write
        case 'W':
        case 'x': // execute
        case 'X':
            value |= (0x0001 << (len - 1 - i));
            break;
        }
    }
    return value;
}

void octls(char *code, unsigned short value) {
    // char type code array length is 10
    for (int i = 0; i < 9; i++) {
        if (value & (0x0001 << (8 - i))) {
            switch (i % 3) {
            case 0:
                code[i] = 'r';
                break;
            case 1:
                code[i] = 'w';
                break;
            case 2:
                code[i] = 'x';
                break;
            }
        }
        else code[i] = '-';
    }
    code[9]='\0';
}

// 3-3
int main(int argc, char *argv[]) {

    char code[10] = {0};
    unsigned short value = lsock("r-xrwxr-x");

    printf("permission code to octal code : %o\n",value);
    octls(code,value);
    printf("octal code to char * : %s\n",code);

    return 0;
}
