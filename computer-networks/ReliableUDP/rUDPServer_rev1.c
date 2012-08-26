/*
 * rUDPServer.c
 *
 *  Created on: Nov 30, 2009
 *      Author: Deepak Konidena
 */

# include <stdio.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>

# define MAXBUFFER 255

int main(int argc, char **argv){

	int serverSockDesc;
	struct sockaddr_in serverSockAddr;
	struct sockaddr_in clientSockAddr;
	unsigned int clientSockAddrLen;
	char sBuffer[MAXBUFFER];
	unsigned short serverPort;
	int recvMsgSize;

	if(argc != 2){
		fprintf(stderr,"\nUsage: %s <UDP SERVER PORT>", argv[0]);
		exit(1);
	}

	serverPort = atoi(argv[1]);

	if((serverSockDesc = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		fprintf(stderr,"\n socket() failed");

	memset(&serverSockAddr,0,sizeof(serverSockAddr));
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverSockAddr.sin_port = htons(serverPort);

	if(bind(serverSockDesc,(struct sockaddr *)&serverSockAddr,sizeof(serverSockAddr)) < 0)
		fprintf(stderr,"\n bind() failed");

	for(;;){

		clientSockAddrLen = sizeof(clientSockAddr);

		if((recvMsgSize = recvfrom(serverSockDesc, sBuffer, MAXBUFFER, 0, (struct sockaddr *)&clientSockAddr, &clientSockAddrLen)) < 0)
			fprintf(stderr,"\n recvfrom() failed");

		printf("\nConnected to client %s\n",inet_ntoa(clientSockAddr.sin_addr));

		if(sendto(serverSockDesc,sBuffer,recvMsgSize,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= recvMsgSize)
			fprintf(stderr,"\n sendto() sent a different number of bytes than originally expected! ");
	}

	return 0;
}
