#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handle(char *msg) {
    perror(msg);
    exit(2);
}

int main(int argc, char *argv[]) {
    int sock;
    int client_addr_size;
    struct sockaddr_in server_addr, client_addr;
    char buff_rcv[BUF_SIZE];
    char buff_snd[BUF_SIZE];

    sock = socket(PF_INET,SOCK_DGRAM,0);
    if (sock == -1) {
        printf("socket() error");
        exit(1);
    }

    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;    
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1) {
        printf("bind() error");
        exit(1);
    }

    while(1) {
        client_addr_size = sizeof(client_addr);
        recvfrom(sock,buff_rcv,BUF_SIZE,0,(struct sockaddr *)&client_addr,&client_addr_size);
        printf("receive : %s \n",buff_rcv);

        sprintf(buff_snd,"%s%s\n",buff_rcv,buff_rcv);
        sendto(sock,buff_snd,strlen(buff_snd)+1,0,(struct sockaddr *)&client_addr, sizeof(client_addr));

    }
    return 0;
}
