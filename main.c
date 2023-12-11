#include <unistd.h>
#include "fonction.h"

int main(int argc , char **argv) {
    char buffer[128]={0};
    char receive[128]={0};
    char server[128]={0};
    char rrq[128]={0};
    char ACK[128]={0};
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *otherhints= calloc(1,sizeof(struct addrinfo));
    struct sockaddr *socketrecv;
    int soc,socketrecvSize;
    char* Namefile=argv[1];
    char* serv=argv[2];
    char* port=argv[3];

    if(argc!=4)
    {
        fprintf(stderr,"Usage:%s port\n",argv[0]);
    }

    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_flags=0;
    hints.ai_next=NULL;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_family=AF_UNSPEC;
    hints.ai_protocol=IPPROTO_UDP;

    printf("The folder's name is %s\n\r",Namefile);
    printf("The server's name is %s \n\r",serv);
    printf("The port's name is %s\n\r",port);

    getaddrinfo(serv, port, &hints, &res);
    getnameinfo(res->ai_addr,res->ai_addrlen,buffer,128,server,128,NI_NUMERICHOST | NI_NUMERICSERV);

    printf("server :%s:%s\n",buffer,server);
    
    soc=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(soc==-1)
    {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    printf("socket %d\n\r",soc);
    sprintf(rrq, " \1%s netascii",Namefile);
    rrq[0]=0;
    rrq[strlen(Namefile)+2]=0;

    sendto(soc,buffer,128,res->ai_flags,res->ai_addr,res->ai_addrlen);
    recvfrom(soc,receive,128,res->ai_flags,&socketrecv,&socketrecvSize);


    return 0;
}
