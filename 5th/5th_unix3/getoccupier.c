#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "hotel.h"
#include "getoccupier.h"

#define LINELENGTH 4

char buffer[LINELENGTH];

char *getoccupier(int roomno) {
	off_t offset; // offset num
	ssize_t nread; // read char num

	// file read
	if(listfiledes == -1 && (listfiledes = open(listfile,O_RDONLY))==-1) {
		fprintf(stderr, "Error on opening list file\n");
		return NULL;
	}

	// offset operation
	offset = (roomno-1) * LINELENGTH;

	
	//printf("file des : %d",listfiledes);
	//printf("file offset : %d\n",offset);

	// file pointer immigration
	if(lseek(listfiledes,offset,SEEK_SET)==-1) {
		fprintf(stderr,"Error on setting offset \n");
		return NULL;
	}

	if((nread = read(listfiledes,buffer,LINELENGTH))<0) {
		fprintf(stderr,"Error on reading list file \n");
		return NULL;
	}
	buffer[LINELENGTH-1]='\0';
	return buffer;
}
