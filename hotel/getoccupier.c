#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "hotel.h"
#include "getoccupier.h"

#define LINELENGTH 4
#define NROOMS 10

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

int findfree()
{
    int i = 0;
    for(i = 1; i <= NROOMS; i++ )                  // 여기랑
    {
        getoccupier(i);
        if( buffer[0] == ' ')
            return i;
    }
    return (-1);
}

int freeroom(int roomno)
{
    int i;
    off_t offset;

    offset = (roomno -1) * LINELENGTH;

    if(lseek(listfiledes, offset, SEEK_SET) == -1)
        return (-1);

    for(i = 0; i < LINELENGTH - 1; i ++)
        buffer[i] = ' ';
    buffer[LINELENGTH-1] = '\n';

    if( write(listfiledes, buffer, LINELENGTH) != LINELENGTH)
        return (-1);
    return (0);
}

int addguest(int roomno, char *guest)
{
    int i;
    off_t offset;
    offset = (roomno -1)*LINELENGTH;

    getoccupier(roomno);
    if(buffer[0] != ' ')
    {
        printf("ROOM %s is full", roomno);
        return (-1);
    }

    for(i = 0; i < (LINELENGTH -1); i++)
        buffer[i] = ' ';
    buffer[LINELENGTH-1] = '\n';
    i = 0;
    while(guest[i] != '\0')
    {
        buffer[i] = guest[i];
    }

    if( lseek(listfiledes, offset, SEEK_SET) == -1)
        return (-1);

    if(write(listfiledes, buffer, LINELENGTH) <= 0)
        return (-1);

    return 0;
}
