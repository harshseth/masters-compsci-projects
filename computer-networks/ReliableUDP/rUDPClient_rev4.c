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
# define PACKETSIZE 512
# define FILENAME 10
# define ACKSIZE 16
# define SEGMENTSIZE 528


struct rUDPHeader{
	unsigned short int FIN;
	unsigned short int finalOffset;
	unsigned short int ackNumber;
	unsigned short int seqNumber;
}*rudphdr;


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
	static char cBuffer[MAXBUFFER], ackBuffer[ACKSIZE], cSegmentBuffer[SEGMENTSIZE];
	int MsgLen, listenFlag = 1;
	static int recvMsgLen, ackLen;
	rudphdr = (struct rUDPHeader *)calloc(1,sizeof(struct rUDPHeader)) ;
	char *initializer = "";
	char *cPacketBuffer = (char *)calloc(1,PACKETSIZE*sizeof(char));

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

		sleep(1);
		recvMsgLen = sizeof(fromSockAddr);

		if((recvMsgLen = recvfrom(clientSockDesc, cSegmentBuffer, SEGMENTSIZE, 0, (struct sockaddr *)&fromSockAddr, &recvMsgLen))!= SEGMENTSIZE)
			fprintf(stderr,"\nrecvfrom() failed because the received length is incorrect");


		//packetNumber++;
		//Reading header and data
		sscanf(&cSegmentBuffer[0],"%d",(int *)&rudphdr->FIN);
		sscanf(&cSegmentBuffer[4],"%d",(int *)&rudphdr->finalOffset);
		sscanf(&cSegmentBuffer[8],"%d",(int *)&rudphdr->ackNumber);
		sscanf(&cSegmentBuffer[12],"%d",(int *)&rudphdr->seqNumber);
		strcpy(cPacketBuffer,&cSegmentBuffer[16]);

		rudphdr->ackNumber = rudphdr->seqNumber;

		printf("\nReceived packet%d %s!", rudphdr->seqNumber, cPacketBuffer);
		sprintf(&ackBuffer[0],"%d",(int)rudphdr->FIN);
		sprintf(&ackBuffer[4],"%d",(int )rudphdr->finalOffset);
		sprintf(&ackBuffer[8],"%d",(int )rudphdr->ackNumber);
		sprintf(&ackBuffer[12],"%d",(int )rudphdr->seqNumber);

		if(sendto(clientSockDesc, ackBuffer, ACKSIZE, 0,(struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr))!= ACKSIZE)
				fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected!");

		printf("\nSent ACK for packet%d %s!", rudphdr->ackNumber, cPacketBuffer);



			if((fileWriteLen = fwrite (cPacketBuffer,1,PACKETSIZE,pFile)) != PACKETSIZE){
				fprintf (stderr,"\nReading fwrite() error");
				exit (3);
			}


		strcpy(cPacketBuffer,initializer);

		if(rudphdr->FIN == 1 ){
			fclose(pFile);
			break;
		}

	}

	close(clientSockDesc);
	//fclose(pFile);
	printf("\n");
	exit(0);

}
