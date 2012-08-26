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
# define PACKETSIZE 1

int main(int argc, char **argv){

	FILE * pFile;
	long int fileSize, recvMsgSize, tempSize;
	size_t fileReadLen;
	int serverSockDesc, i, packetNumber = 1, listenFlag = 1;
	struct sockaddr_in serverSockAddr;
	struct sockaddr_in clientSockAddr;
	unsigned int clientSockAddrLen;
	static char sBuffer[MAXBUFFER], fileName[50], sPacketBuffer[PACKETSIZE], ackBuffer[10];
	unsigned short serverPort;
	int fileNameLen, ackLen;
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
		fprintf(stderr,"\nbind() failed");

	fileNameLen = sizeof(clientSockAddr);
	if((fileNameLen = recvfrom(serverSockDesc, fileName, 50, 0, (struct sockaddr *)&clientSockAddr, &fileNameLen)) < 0)
		fprintf(stderr,"\nrecvfrom() failed");

	printf("\nConnected to client %s\n",inet_ntoa(clientSockAddr.sin_addr));
	printf("\nFile that needs to be retrieved is %s\n", fileName);

	if ((pFile = fopen ( fileName , "rb" )) == NULL){
		fprintf(stderr,"\nFile opening fopen() error");
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

	if((fileReadLen = fread (sBuffer,1,fileSize,pFile)) != fileSize){
		fprintf (stderr,"\nReading fread() error");
		exit (3);
	}
	*/
	while(listenFlag){
		while(!feof(pFile)){

			tempSize = fileSize - ftell(pFile);

			if(tempSize == 0)
				break;

			if(tempSize < PACKETSIZE){
				if((fileReadLen = fread (sPacketBuffer,1,tempSize,pFile)) != tempSize){
					fprintf(stderr,"\nfread() error");
				}
				//Padding
				i = 0;
				for(i= tempSize;i<PACKETSIZE-1;i++)
					sPacketBuffer[i] = '@';
				//Sending packets
				if(sendto(serverSockDesc,sPacketBuffer,PACKETSIZE,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= PACKETSIZE)
					fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected! ");
				printf("\nPacket%d sent!",packetNumber++);

			}
			else{
				if((fileReadLen = fread (sPacketBuffer,1,PACKETSIZE,pFile)) != PACKETSIZE){
					fprintf(stderr,"\nfread() error");
				}
				//Sending packets
				if(sendto(serverSockDesc,sPacketBuffer,PACKETSIZE,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= PACKETSIZE)
					fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected! ");
				printf("\nPacket%d sent!",packetNumber++);
			}

		}

		ackLen = sizeof(clientSockAddr);
		if((fileNameLen = recvfrom(serverSockDesc, ackBuffer, 10, 0, (struct sockaddr *)&clientSockAddr, &ackLen)) < 0)
			fprintf(stderr,"\nrecvfrom() failed");
		printf("\nAck received for Packet%s!", ackBuffer);

		if(atoi(ackBuffer) == 498)
			listenFlag = 0;

	}

	/*if(sendto(serverSockDesc,sBuffer,fileSize,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= fileSize)
		fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected! ");

	for(;;){

		clientSockAddrLen = sizeof(clientSockAddr);

		if((recvMsgSize = recvfrom(serverSockDesc, sBuffer, MAXBUFFER, 0, (struct sockaddr *)&clientSockAddr, &clientSockAddrLen)) < 0)
			fprintf(stderr,"\n recvfrom() failed");

		printf("\nConnected to client %s\n",inet_ntoa(clientSockAddr.sin_addr));

		if(sendto(serverSockDesc,sBuffer,recvMsgSize,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= recvMsgSize)
			fprintf(stderr,"\n sendto() sent a different number of bytes than originally expected! ");
	}
	*/

	fclose(pFile);
	printf("\n");
	return 0;
}

