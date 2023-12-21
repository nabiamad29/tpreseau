#include "fonction.h"

void gettftp(int argc , char **argv)
{
    char buffer[128]={0};
    char receive[1024]={0};
    char server[128]={0};
    char rrq[128]={0};
    char ACK[4]={0};
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *otherhints= calloc(1,sizeof(struct addrinfo));
    struct sockaddr socketrecv;
    int soc;
    socklen_t socketrecvSize;
    socketrecvSize=sizeof(struct sockaddr_storage);
    char* Namefile=argv[1];
    char* serv=argv[2];
    char* port=argv[3];
    int fd;


    fd=open(Namefile,O_WRONLY|O_CREAT|O_EXCL,S_IRWXU|S_IRWXG|S_IRWXO);

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
    sprintf(rrq, " \1%s octet",Namefile);
    int sizeBufferToSend=strlen(rrq)+1;
    rrq[0]=0;
    rrq[strlen(Namefile)+2]=0;

    int status = sendto(soc,rrq,sizeBufferToSend,res->ai_flags,res->ai_addr,res->ai_addrlen);
    printf("status sendto = %d\n",status);
    ACK[0]=0;
    ACK[1]=4;
    int blockNumberReceived;
    int blockNumber=1;
    do {
        status = recvfrom(soc,receive,1024,0,&socketrecv,&socketrecvSize);
        blockNumberReceived= receive[2]*256+receive[3];
        printf("status recvfrom = %d, blockNumber = %d\n",status, blockNumberReceived);
        if(blockNumberReceived==blockNumber){
            write(fd,receive+4,status-4);
            blockNumber++;
        }
        ACK[2]=receive[2];
        ACK[3]=receive[3];
        //  sleep(1);
        int statusACK = sendto(soc,ACK,4,0,&socketrecv,socketrecvSize);
        printf("status n°2 sendto = %d\n",statusACK);
    }
    while(status==516);

    close(fd);
 }

void puttftp(int argc , char **argv) {
    char buffer[128]={0};
    char send[1024]={0};
    char server[128]={0};
    char wrq[128]={0};
    char ACK[1024]={0};
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *otherhints= calloc(1,sizeof(struct addrinfo));
    struct sockaddr socketrecv;
    int soc;
    socklen_t socketrecvSize;
    socketrecvSize=sizeof(struct sockaddr_storage);
    char* Namefile=argv[1];
    char* serv=argv[2];
    char* port=argv[3];
    int fd;


    fd=open(Namefile,O_WRONLY|O_CREAT|O_EXCL,S_IRWXU|S_IRWXG|S_IRWXO);

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
    sprintf(wrq, " \2%s octet",Namefile);
    int sizeBufferToSend=strlen(wrq)+1;
    wrq[0]=0;
    wrq[strlen(Namefile)+2]=0;

    int status = sendto(soc,wrq,sizeBufferToSend,res->ai_flags,res->ai_addr,res->ai_addrlen);
    printf("status sendto = %d\n",status);
    status = recvfrom(soc,ACK,1024,0,&socketrecv,&socketrecvSize);
    printf("Request ACK %d\n",status);
    int blockNumberSend;
    //int blockNumber=1;
    do {
        status = sendto(soc,send,1024,res->ai_flags,res->ai_addr,res->ai_addrlen);
        //blockNumberSend= send[2]*256+send[3];
        printf("status sendTo = %d, blockNumber = %d\n",status, blockNumberSend);
        /*if(blockNumberSend==blockNumber){
            write(fd,send+4,status-4);
            blockNumber++;
        }*/
        //  sleep(1);
        int statusACK = recvfrom(soc,ACK,4,0,&socketrecv,socketrecvSize);
        printf("status n°2 recvFrom = %d\n",statusACK);
    }
    while(status==516);

    close(fd);

}