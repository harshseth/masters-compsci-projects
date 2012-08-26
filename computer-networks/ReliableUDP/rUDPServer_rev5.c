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
# define ACKSIZE 16
# define SEGMENTSIZE (PACKETSIZE + 16)

struct rUDPHeader{
	unsigned int FIN;
	unsigned int finalOffset;
	unsigned int ackNumber;
	unsigned int seqNumber;
}*rudphdr;


int main(int argc, char **argv){

	FILE * pFile;
	long int fileSize, recvMsgSize, tempSize;
	size_t fileReadLen;
	int serverSockDesc, i, packetNumber = 1, listenFlag = 1;
	struct sockaddr_in serverSockAddr;
	struct sockaddr_in clientSockAddr;
	unsigned int clientSockAddrLen;
	static char sBuffer[MAXBUFFER], fileName[50], sPacketBuffer[PACKETSIZE], ackBuffer[ACKSIZE], sSegmentBuffer[SEGMENTSIZE];
	unsigned short serverPort;
	int fileNameLen, ackLen, lastSentSeqNum = 0, isEOF = 1;
	rudphdr = (struct rUDPHeader *)calloc(1,sizeof(struct rUDPHeader)) ;


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


	while(listenFlag){
		if(isEOF != -1){

			tempSize = fileSize - ftell(pFile);

			if(tempSize == 0 || tempSize < PACKETSIZE)
				isEOF = -1;


			if(tempSize < PACKETSIZE){
				if((fileReadLen = fread (sPacketBuffer,1,tempSize,pFile)) != tempSize){
					fprintf(stderr,"\nfread() error");
				}
				//Padding
				i = 0;
				for(i= tempSize;i<PACKETSIZE-1;i++)
					sPacketBuffer[i] = '@';

				//populating the header
				rudphdr->FIN = 1;
				rudphdr->finalOffset = (unsigned int)tempSize;
				rudphdr->ackNumber = 0;
				rudphdr->seqNumber = packetNumber;
				lastSentSeqNum = rudphdr->seqNumber;
				packetNumber++;

				sprintf(&sSegmentBuffer[0],"%d",(int)rudphdr->FIN);
				sprintf(&sSegmentBuffer[4],"%d",(int)rudphdr->finalOffset);
				sprintf(&sSegmentBuffer[8],"%d",(int)rudphdr->ackNumber);
				sprintf(&sSegmentBuffer[12],"%d",(int)rudphdr->seqNumber);
				sprintf(&sSegmentBuffer[16],"%s",sPacketBuffer);

				//Sending packets
				if(sendto(serverSockDesc,sSegmentBuffer,SEGMENTSIZE,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= SEGMENTSIZE)
					fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected! ");
				printf("\nPacket%d finalOffset:%d sent!",rudphdr->seqNumber, rudphdr->finalOffset);

			}
			else{
				if((fileReadLen = fread (sPacketBuffer,1,PACKETSIZE,pFile)) != PACKETSIZE){
					fprintf(stderr,"\nfread() error");
				}

				//populating the header
				rudphdr->FIN = 0;
				rudphdr->finalOffset = 0;
				rudphdr->ackNumber = 0;
				rudphdr->seqNumber = packetNumber;
				lastSentSeqNum = rudphdr->seqNumber;
				packetNumber++;

				sprintf(&sSegmentBuffer[0],"%d",(int)rudphdr->FIN);
				sprintf(&sSegmentBuffer[4],"%d",(int)rudphdr->finalOffset);
				sprintf(&sSegmentBuffer[8],"%d",(int)rudphdr->ackNumber);
				sprintf(&sSegmentBuffer[12],"%d",(int)rudphdr->seqNumber);
				sprintf(&sSegmentBuffer[16],"%s",sPacketBuffer);

				//Sending packets
				if(sendto(serverSockDesc,sSegmentBuffer,SEGMENTSIZE,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= SEGMENTSIZE)
					fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected! ");
				printf("\nPacket%d finalOffset:%d sent!",rudphdr->seqNumber, rudphdr->finalOffset);
			}


		}

		ackLen = sizeof(clientSockAddr);
		if((fileNameLen = recvfrom(serverSockDesc, ackBuffer, ACKSIZE, 0, (struct sockaddr *)&clientSockAddr, &ackLen)) != ACKSIZE )
			fprintf(stderr,"\nrecvfrom() failed");
		sscanf(&ackBuffer[0],"%d",(int *)&rudphdr->FIN);
		sscanf(&ackBuffer[4],"%d",(int *)&rudphdr->finalOffset);
		sscanf(&ackBuffer[8],"%d",(int *)&rudphdr->ackNumber);
		sscanf(&ackBuffer[12],"%d",(int *)&rudphdr->seqNumber);
		printf("\nAck received for Packet%d!", rudphdr->ackNumber);

		if(rudphdr->ackNumber == lastSentSeqNum && isEOF == -1)
			listenFlag = 0;

	}



	fclose(pFile);
	printf("\n");
	return 0;
}

