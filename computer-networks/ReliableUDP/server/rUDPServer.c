/*
 * rUDPServer.c
 *
 *  Created on: Nov 29, 2009
 *      Author: deepakkoni
 */

# include <stdio.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>

#define MAXBUF 1024*1024

struct sockaddr_in *clientSockAddr, *serverSockAddr;
socklen_t clientSockLen, serverSockLen;


void echo( int ld ) {
    int n;
    char sBuffer[MAXBUF];


    clientSockLen = sizeof(clientSockAddr);

    while (1) {

    	n=recvfrom(ld,sBuffer,MAXBUF,0,(struct sockaddr *)clientSockAddr,&clientSockLen);
    	printf("Got a datagram from %s port %d\n", inet_ntoa(clientSockAddr->sin_addr), ntohs(clientSockAddr->sin_port));

      if (n<0)
    	  fprintf(stderr,"\nError receiving data");
       else {
    	   printf("GOT %d BYTES\n",n);
    	   sendto(ld,sBuffer,n,0,(struct sockaddr *)clientSockAddr,clientSockLen);
       }

    }
}


int main(int argc, char **argv){

	int socketDescriptor;
	serverSockAddr = (struct sockaddr_in *)calloc(1,sizeof(struct sockaddr_in));
	clientSockAddr = (struct sockaddr_in *)calloc(1,sizeof(struct sockaddr_in));


	//create a socket
	if((socketDescriptor = socket(PF_INET,SOCK_DGRAM,0)) < 0){
		fprintf(stderr,"\n Problem creating socket \n");
		exit(1);
	}

	//establishing the address family, IP Address and Port number
	serverSockAddr->sin_family = AF_INET;
	serverSockAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	serverSockAddr->sin_port = htons(0);



	if(bind(socketDescriptor, (struct sockaddr *)serverSockAddr, sizeof(struct sockaddr_in)) < 0){
		fprintf(stderr,"Problem binding\n");
		exit(0);
	}

	serverSockLen = sizeof(struct sockaddr_in);

	if(getsockname(socketDescriptor,(struct sockaddr *)serverSockAddr, &serverSockLen) < 0){
		fprintf(stderr,"Error getsockname \n");
		exit(1);
	}

	//converting port numbers to host byte order
	printf("The server's UDP port number is %d \n", ntohs(serverSockAddr->sin_port));
	printf("The server's IPAddress is %s\n",inet_ntoa(serverSockAddr->sin_addr));

	// Echo every datagram that we get
	echo(socketDescriptor);

	return(0);
}
