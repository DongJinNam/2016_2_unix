#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

//define
#define LINELENGTH	4
#define NROOMS		10

//variable
char buffer[LINELENGTH];
int infile = -1;

//Function
char *getoccupier(int roomno);
int findfree(void);
int freeroom(int );
int addguest(int , char * );

int main(int argc, char *argv[])
{
    char command, inputbuf[LINELENGTH];
    int nroom;

    if( (infile = open("listfile", O_RDWR | O_CREAT)) == -1)
    {
        printf("can't open file");
        exit(1);
    }
    while(1)
    {
        printf("Add guest : a,Free Room : f,Get guest name : g, Quit : q\n");
        printf("input : ");
        scanf("%c",&command);
        switch(command)
        {
            case 'a' :
                printf("input Occupier's Name: ");
                scanf("%s",inputbuf);
                nroom = findfree();
                if (nroom == -1)
                    printf("Cannot input.\n");
                else
                    addguest(nroom, inputbuf);
                break;
            case 'f' :
                printf("input Room number : ");
                scanf("%d",&nroom);
                if(freeroom(nroom))
                    printf("can't free room\n");
                else
                    printf("Success free room\n");
                break;
            case 'g' :
                printf("input Room number : ");
                scanf("%d",&nroom);
                if(getoccupier(nroom))
                    printf("%s in ROOM %d\n", buffer, nroom);
                else
                    printf("ROOM %d is empty\n", nroom);
                break;
            case 'q' :
                printf("quit this program bye~\n");
                close(infile);
                exit(0);
            default :
                break;
        }
        fflush(stdout);
    }
}

char *getoccupier(int roomno)
{
    off_t offset; // offset num
    ssize_t nread; // read char num
    int count = 0; // character count(not ' ')

    // offset operation
    offset = (roomno-1) * LINELENGTH;

    // file pointer immigration
    if(lseek(infile,offset,SEEK_SET)==-1) {
        return NULL;
    }
    if((nread = read(infile,buffer,LINELENGTH))<0) {
        return NULL;
    }
    // buffer is null or not null seperation
    for (int i=0; i<LINELENGTH-1;i++) {
        if(buffer[i] != ' ') count++;
    }
    if (count == 0) return NULL;

    buffer[LINELENGTH-1]='\0';
    return buffer;
}

int findfree()
{
    int i = 0;
    for(i = 1; i <= NROOMS; i++ )                  // 여기랑
    {
        if(getoccupier(i) == NULL)
            return i;
    }
    return -1;
}

int freeroom(int roomno)
{
    int i;
    off_t offset;

    offset = (roomno -1) * LINELENGTH;

    if( lseek(infile, offset, SEEK_SET) == -1)
        return -1;

    for(i = 0; i < LINELENGTH - 1; i ++)
        buffer[i] = ' ';
    buffer[LINELENGTH-1] = '\n';

    if(write(infile, buffer, LINELENGTH) != LINELENGTH)
        return -1;
    return 0;
}

int addguest(int roomno, char *guest)
{
    int i;
    off_t offset;
    offset = (roomno -1)*LINELENGTH;

    if(getoccupier(roomno))
    {
        printf("ROOM %d is full\n", roomno);
        return (-1);
    }
    for(i = 0; i < (LINELENGTH -1); i++)
        buffer[i] = ' ';
    buffer[LINELENGTH-1] = '\n';
    i = 0;
    while(guest[i] != '\0')
    {
        buffer[i] = guest[i];
        i++;
    }
    if( lseek(infile, offset, SEEK_SET) == -1)
        return (-1);

    if(write(infile, buffer, LINELENGTH) <= 0)
        return (-1);
    return 0;
}
