#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "copyfile.h"
#define BUFSIZE 512
#define PERM 0644

int copyfile(const char *in, const char *out) {
	int infile,outfile;
	int nread;
	char buffer[BUFSIZE];

	if((infile = open(in,O_RDONLY)) < 0) {
		printf("infile open error\n");
		return -1;
	}
	if((outfile = open(out, O_WRONLY | O_CREAT | O_TRUNC, PERM)) < 0) {
		printf("outfile open error\n");
		return -1;
	}
	/*
	if((outfile = open(out, O_WRONLY | O_CREAT | O_APPEND, PERM)) < 0) {
		printf("outfile open error\n");
		return -1;
	}
	*/
	while((nread = read(infile, buffer, BUFSIZE)) > 0) {
		if (write(outfile, buffer, nread) != nread) {
			printf("couldn't write while buffer\n");
		}
	}

	close(infile);
	close(outfile);

	if (nread == -1) {
		printf("error on read\n");
		return -4;
	}
	else
		return 0;
}
