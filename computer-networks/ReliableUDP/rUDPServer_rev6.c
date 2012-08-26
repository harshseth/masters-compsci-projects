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
# include <sys/time.h>
# include <sys/select.h>
# include <fcntl.h>
# include <math.h>

# define MAXBUFFER 65000
# define PACKETSIZE 512
# define ACKSIZE 16
# define SEGMENTSIZE (PACKETSIZE + 16)
# define SEGMENTNUM 50
# define DELTA 0.25
# define MU 1
# define PHI 4

struct rUDPHeader{
	unsigned int FIN;
	unsigned int finalOffset;
	unsigned int ackNumber;
	unsigned int seqNumber;
}*rudphdr;

struct sendingBuffer{
	struct timeval start, end;
	char segment[SEGMENTSIZE];
	double deviation, estimatedRTT, sampleRTT, difference;
	int flag;
}sCache[SEGMENTNUM];

int main(int argc, char **argv){

	FILE * pFile;
	fd_set fds;
	struct timeval timeOut;
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
	int LAR = 1, LFS = 1, SWS = 4;
	int retVal = 0;
	long flags; // for blocking and non-blocking sockets


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

	//setting socket descriptor
	FD_ZERO(&fds);
	FD_SET(serverSockDesc,&fds);

	//define timeout
	timeOut.tv_sec = 5;
	timeOut.tv_usec = 0;

	//Initial values for sCache
	sCache[0].deviation = 0;
	sCache[0].estimatedRTT = 0;
	sCache[0].sampleRTT = 0;
	sCache[0].difference = 0;

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
			if((LFS - LAR) <= SWS){
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
					rudphdr->seqNumber = LFS;
					lastSentSeqNum = rudphdr->seqNumber;
					//LFS = rudphdr->seqNumber;
					gettimeofday(&sCache[rudphdr->seqNumber].start, NULL);
					//packetNumber++;

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
					rudphdr->seqNumber = LFS;
					lastSentSeqNum = rudphdr->seqNumber;
					//LFS = rudphdr->seqNumber;
					gettimeofday(&sCache[rudphdr->seqNumber].start, NULL);
					//packetNumber++;

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
		}
		//set socket to blocking
		flags = fcntl(serverSockDesc, F_GETFL, NULL);
		flags &= (~O_NONBLOCK);
		fcntl(serverSockDesc, F_SETFL, flags);

		//select() sets the timeout and waits for the packets to arrive
		if((retVal= select(serverSockDesc+1,&fds,NULL,NULL,&timeOut)) == -1)
			fprintf(stderr,"\nSelect() failed");

		else if(FD_ISSET(serverSockDesc,&fds)){

			//set socket to non-blocking
			flags = fcntl(serverSockDesc, F_GETFL, NULL);
			flags |= O_NONBLOCK;
			fcntl(serverSockDesc, F_SETFL, flags);

			ackLen = sizeof(clientSockAddr);
			if((fileNameLen = recvfrom(serverSockDesc, ackBuffer, ACKSIZE, 0, (struct sockaddr *)&clientSockAddr, &ackLen)) != ACKSIZE )
				fprintf(stderr,"\nrecvfrom() failed");
			sscanf(&ackBuffer[0],"%d",(int *)&rudphdr->FIN);
			sscanf(&ackBuffer[4],"%d",(int *)&rudphdr->finalOffset);
			sscanf(&ackBuffer[8],"%d",(int *)&rudphdr->ackNumber);
			sscanf(&ackBuffer[12],"%d",(int *)&rudphdr->seqNumber);
			LAR = rudphdr->ackNumber;

			gettimeofday(&sCache[rudphdr->seqNumber].end, NULL);
			printf("\nAck received for Packet%d!", rudphdr->ackNumber);
			printf("\nRound Trip Time: %.2lf",  ((double)sCache[rudphdr->seqNumber].end.tv_sec + (double)((double)sCache[rudphdr->seqNumber].end.tv_usec/ 1000000.0)) - ((double)sCache[rudphdr->seqNumber].start.tv_sec + (double)((double)sCache[rudphdr->seqNumber].start.tv_usec/ 1000000.0)));

			// Jacobson/ Karels Timeout calculation
			sCache[rudphdr->seqNumber].sampleRTT = ((double)sCache[rudphdr->seqNumber].end.tv_sec + (double)((double)sCache[rudphdr->seqNumber].end.tv_usec/ 1000000.0)) - ((double)sCache[rudphdr->seqNumber].start.tv_sec + (double)((double)sCache[rudphdr->seqNumber].start.tv_usec/ 1000000.0));
			sCache[rudphdr->seqNumber].difference = sCache[rudphdr->seqNumber].sampleRTT - sCache[rudphdr->seqNumber - 1].estimatedRTT;
			sCache[rudphdr->seqNumber].estimatedRTT = sCache[rudphdr->seqNumber - 1].estimatedRTT + (double)(DELTA) * sCache[rudphdr->seqNumber].difference;
			sCache[rudphdr->seqNumber].deviation = sCache[rudphdr->seqNumber - 1].deviation + (double)(DELTA) * (fabs((sCache[rudphdr->seqNumber].difference)) - sCache[rudphdr->seqNumber - 1].deviation);

			printf("\nSampleRTT: %0.2lf EstimatedRTT: %0.2lf Deviation: %0.2lf Difference: %0.2lf",sCache[rudphdr->seqNumber].sampleRTT,sCache[rudphdr->seqNumber].estimatedRTT,sCache[rudphdr->seqNumber].deviation,sCache[rudphdr->seqNumber].difference);
			timeOut.tv_sec = MU * sCache[rudphdr->seqNumber].estimatedRTT + PHI * sCache[rudphdr->seqNumber].deviation;
			timeOut.tv_usec = 0;

			if(timeOut.tv_sec < 1)
				timeOut.tv_sec = 5;
			printf("\nTimeout value for next packet is %.2lf",(double)timeOut.tv_sec );
			printf("\nLFS: %d, LAR: %d",LFS,LAR);
			printf("\n---------------------------------------");

			if(LAR == (LFS + 1))
				LFS = LFS + 1;
		}

		else if(retVal == 0){
					fprintf(stderr,"\nTimer expired for Packet%d\n",rudphdr->seqNumber);
					printf("\nResending....");
		}


		if(rudphdr->seqNumber == lastSentSeqNum && isEOF == -1)
			listenFlag = 0;
	}



	fclose(pFile);
	printf("\n");
	return 0;
}
