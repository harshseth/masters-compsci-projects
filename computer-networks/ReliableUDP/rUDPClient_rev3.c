/*
 * rUDPClient_rev2.c
 *
 *  Created on: Nov 30, 2009
 *      Author: deepakkoni
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
# define FILENAME 10

int main(int argc, char **argv){

	static FILE * pFile;
	long int fileSize, tempSize;
	size_t fileWriteLen;
	int clientSockDesc, i, packetNumber = 1;
	struct sockaddr_in serverSockAddr;
	struct sockaddr_in fromSockAddr;
	unsigned short serverPort;
	unsigned int fromSockLen;
	char *serverIPAddr = (char *)calloc(1,50*sizeof(char));
	char *Msg = (char *)calloc(1,FILENAME*sizeof(char));
	static char fileName[20] ;
	static char cBuffer[MAXBUFFER], cPacketBuffer[PACKETSIZE], ackBuffer[FILENAME];
	int MsgLen, listenFlag = 1;
	static int recvMsgLen, ackLen;

	if( (argc < 4) || (argc > 5)){
		fprintf(stderr,"\nUsage: %s <Server IP> <Message> <Server Port>", argv[0]);
		exit(1);
	}

	serverIPAddr = argv[1];
	Msg = argv[2];
	serverPort = atoi(argv[3]);

	if((MsgLen = strlen(Msg)) > MAXBUFFER)
		fprintf(stderr,"\nMessage too long");

	//Creating the filename
	strcpy(fileName,"t");
	strcat(fileName,Msg);

	if((clientSockDesc = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		fprintf(stderr,"\nsocket() failed");

	memset(&serverSockAddr, 0, sizeof(serverSockAddr));
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_addr.s_addr = inet_addr(serverIPAddr);
	serverSockAddr.sin_port = htons(serverPort);

	if(sendto(clientSockDesc, Msg, MsgLen, 0,(struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr))!= MsgLen)
		fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected!");

	//create the file
	if ((pFile = fopen ( fileName , "wb" )) == NULL){
		fprintf(stderr,"\nFile opening fopen() error");
		exit(1);
	}
	printf("\nCreated file %s", fileName);

	while(listenFlag){

		sleep(0.1);
		recvMsgLen = sizeof(fromSockAddr);

		if((recvMsgLen = recvfrom(clientSockDesc, cPacketBuffer, PACKETSIZE, 0, (struct sockaddr *)&fromSockAddr, &recvMsgLen))!= PACKETSIZE)
			fprintf(stderr,"\nrecvfrom() failed because the received length is incorrect");

		//cBuffer[recvMsgLen] = '\0';
		//printf("\nReceived: %s\n", cBuffer);
		printf("\nReceived packet%d %s!", packetNumber, cPacketBuffer);
		sprintf(ackBuffer,"%d",packetNumber);

		if(sendto(clientSockDesc, ackBuffer, FILENAME, 0,(struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr))!= FILENAME)
				fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected!");

		printf("\nSent ACK for packet%d %s!", packetNumber++, cPacketBuffer);


		if((fileWriteLen = fwrite (cPacketBuffer,1,PACKETSIZE,pFile)) != PACKETSIZE){
			fprintf (stderr,"\nReading fwrite() error");
			exit (3);
		}
		if(packetNumber == 500){
			fclose(pFile);
			break;
		}

	}

	close(clientSockDesc);
	//fclose(pFile);
	printf("\n");
	exit(0);

}
