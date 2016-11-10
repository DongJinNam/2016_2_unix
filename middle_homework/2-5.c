#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int filedes;
    char buffer[BUFSIZ];
    ssize_t nread;
    int tot_lines = 0;     /* total lines */
    int tot_words = 0;     /* total words */

    if ((filedes = open("anotherfile",O_RDONLY)) == -1) {
        printf("error in opening anotherfile\n");
        exit(1);
    }

    while((nread = read(filedes,buffer,BUFSIZ)) > 0) {
        for (int i = 0; i < nread; i++) {
            if (isspace(buffer[i+1]) && !isspace(buffer[i])) {
                tot_words++;
            }
            if (buffer[i] == '\n') {
                tot_lines++;
            }
            if (buffer[i] == '-') {
                tot_words--;
            }
        }
    }

    printf("Words count : %d, Line count : %d\n", tot_words, tot_lines);

    close(filedes);
    return 0;
}
