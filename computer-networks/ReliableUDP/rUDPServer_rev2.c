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

# define MAXBUFFER 65000

int main(int argc, char **argv){

	FILE * pFile;
	long int fileSize, recvMsgSize;
	size_t fileReadLen;
	int serverSockDesc;
	struct sockaddr_in serverSockAddr;
	struct sockaddr_in clientSockAddr;
	unsigned int clientSockAddrLen;
	char sBuffer[MAXBUFFER], fileName[50];
	unsigned short serverPort;
	int fileNameLen;
	//char *buffer;

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

	fileNameLen = sizeof(clientSockAddr);
	if((fileNameLen = recvfrom(serverSockDesc, fileName, 50, 0, (struct sockaddr *)&clientSockAddr, &fileNameLen)) < 0)
		fprintf(stderr,"\n recvfrom() failed");

	printf("\nConnected to client %s\n",inet_ntoa(clientSockAddr.sin_addr));
	printf("\n File that needs to be retrieved is %s\n", fileName);

	if ((pFile = fopen ( fileName , "rb" )) == NULL){
		fprintf(stderr,"\n File opening fopen() error");
		exit(1);
	}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	fileSize = ftell (pFile);
	rewind (pFile);

	/*if(buffer = (char *)calloc(1,fileSize*sizeof(char)) == NULL){
		fprintf(stderr,"\ncalloc() to buffer error");
		exit(2);
	}
	*/
	if((fileReadLen = fread (sBuffer,1,fileSize,pFile)) != fileSize){
		fprintf (stderr,"\nReading fread() error");
		exit (3);
	}

	if(sendto(serverSockDesc,sBuffer,fileSize,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= fileSize)
		fprintf(stderr,"\n sendto() sent a different number of bytes than originally expected! ");

	/*for(;;){

		clientSockAddrLen = sizeof(clientSockAddr);

		if((recvMsgSize = recvfrom(serverSockDesc, sBuffer, MAXBUFFER, 0, (struct sockaddr *)&clientSockAddr, &clientSockAddrLen)) < 0)
			fprintf(stderr,"\n recvfrom() failed");

		printf("\nConnected to client %s\n",inet_ntoa(clientSockAddr.sin_addr));

		if(sendto(serverSockDesc,sBuffer,recvMsgSize,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= recvMsgSize)
			fprintf(stderr,"\n sendto() sent a different number of bytes than originally expected! ");
	}
	*/

	fclose(pFile);
	return 0;
}

