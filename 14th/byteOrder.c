#include <stdio.h>
#include <arpa/inet.h>

int main(void)
{
    uint32_t host32 = 0x12345678;
    uint32_t net_32 = htonl(host32);
    uint16_t host16 = 0xABCD;
    uint16_t net_16 = htons(host16);
    unsigned char *hp, *np;
    int i;
    hp = (unsigned char *)&host32;
    np = (unsigned char *)&net_32;
    printf("32-bit : %X -> %X\n",host32,net_32);
    for (i=0;i<4;i++) {
        printf("host:%p net:%p\n",hp,np);
        printf("%02X -> %02X\n",*hp++,*np++);
    }
    printf("16-bit : %X -> %X\n",host16,net_16);
    return 0;
}

