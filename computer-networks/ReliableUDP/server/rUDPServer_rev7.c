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
# define PACKETSIZE 64
# define ACKSIZE 16
# define SEGMENTSIZE (PACKETSIZE + 16)
# define SEGMENTNUM 50
# define DELTA 0.25
# define MU 1
# define PHI 4
# define MSS (8*SEGMENTSIZE)

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
	int serverSockDesc, i,j, packetNumber = 1, listenFlag = 1;
	struct sockaddr_in serverSockAddr;
	struct sockaddr_in clientSockAddr;
	unsigned int clientSockAddrLen;
	static char sBuffer[MAXBUFFER], fileName[50], sPacketBuffer[PACKETSIZE], ackBuffer[ACKSIZE], sSegmentBuffer[SEGMENTSIZE];
	unsigned short serverPort;
	int fileNameLen, ackLen, lastSentSeqNum = 0, isEOF = 1;
	rudphdr = (struct rUDPHeader *)calloc(1,sizeof(struct rUDPHeader)) ;
	int LAR = 1, LFS = 1, SWS = 4;
	int retVal = 0, congWin = SEGMENTSIZE;
	long flags; // for blocking and non-blocking sockets
	int seqNum = 1, seqOffset = 0, receiveFlag = 1, timeoutFlag = 0;
	int totalPacketCount = 0, slowStartPacketCount = 0, addIncreasePacketCount = 0, mulDecreasePacketCount = 0, timeoutCount = 0;


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

	printf("\nConnected to client %s",inet_ntoa(clientSockAddr.sin_addr));
	printf("\nFile that needs to be retrieved is %s", fileName);

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

		for(j = 1; j<= congWin; j= j+ SEGMENTSIZE){
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
						rudphdr->seqNumber = LFS + j/(SEGMENTSIZE);
						lastSentSeqNum = rudphdr->seqNumber;
						totalPacketCount++;
						gettimeofday(&sCache[rudphdr->seqNumber].start, NULL);

						sprintf(&sSegmentBuffer[0],"%d",(int)rudphdr->FIN);
						sprintf(&sSegmentBuffer[4],"%d",(int)rudphdr->finalOffset);
						sprintf(&sSegmentBuffer[8],"%d",(int)rudphdr->ackNumber);
						sprintf(&sSegmentBuffer[12],"%d",(int)rudphdr->seqNumber);
						sprintf(&sSegmentBuffer[16],"%s",sPacketBuffer);

						//Sending packets
						if(sendto(serverSockDesc,sSegmentBuffer,SEGMENTSIZE,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= SEGMENTSIZE)
							fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected! ");
						printf("\nPacket%d sent!",rudphdr->seqNumber);

					}
					else{
						if((fileReadLen = fread (sPacketBuffer,1,PACKETSIZE,pFile)) != PACKETSIZE){
							fprintf(stderr,"\nfread() error");
						}

						//populating the header
						rudphdr->FIN = 0;
						rudphdr->finalOffset = 0;
						rudphdr->ackNumber = 0;
						rudphdr->seqNumber = LFS + j/(SEGMENTSIZE);

						lastSentSeqNum = rudphdr->seqNumber;
						totalPacketCount++;
						gettimeofday(&sCache[rudphdr->seqNumber].start, NULL);

						sprintf(&sSegmentBuffer[0],"%d",(int)rudphdr->FIN);
						sprintf(&sSegmentBuffer[4],"%d",(int)rudphdr->finalOffset);
						sprintf(&sSegmentBuffer[8],"%d",(int)rudphdr->ackNumber);
						sprintf(&sSegmentBuffer[12],"%d",(int)rudphdr->seqNumber);
						sprintf(&sSegmentBuffer[16],"%s",sPacketBuffer);

						//Sending packets
						if(sendto(serverSockDesc,sSegmentBuffer,SEGMENTSIZE,0, (struct sockaddr *)&clientSockAddr, sizeof(clientSockAddr))!= SEGMENTSIZE)
							fprintf(stderr,"\nsendto() sent a different number of bytes than originally expected! ");
						printf("\nPacket%d sent!",rudphdr->seqNumber);

					}
					receiveFlag = 1;
				}
			}
		}

		while(receiveFlag){
			//set socket to blocking
			flags = fcntl(serverSockDesc, F_GETFL, NULL);
			flags &= (~O_NONBLOCK);
			fcntl(serverSockDesc, F_SETFL, flags);

			//select() sets the timeout and waits for the packets to arrive
			if((retVal= select(serverSockDesc+1,&fds,NULL,NULL,&timeOut)) == -1)
				fprintf(stderr,"\nSelect() failed");

			else if(FD_ISSET(serverSockDesc,&fds) || retVal != 0){

				//set socket to non-blocking
				flags = fcntl(serverSockDesc, F_GETFL, NULL);
				flags |= O_NONBLOCK;
				fcntl(serverSockDesc, F_SETFL, flags);

		timeout:
				ackLen = sizeof(clientSockAddr);
				if((fileNameLen = recvfrom(serverSockDesc, ackBuffer, ACKSIZE, 0, (struct sockaddr *)&clientSockAddr, &ackLen)) != ACKSIZE )
					fprintf(stderr,"\nrecvfrom() failed");
				sscanf(&ackBuffer[0],"%d",(int *)&rudphdr->FIN);
				sscanf(&ackBuffer[4],"%d",(int *)&rudphdr->finalOffset);
				sscanf(&ackBuffer[8],"%d",(int *)&rudphdr->ackNumber);
				sscanf(&ackBuffer[12],"%d",(int *)&rudphdr->seqNumber);
				LAR = rudphdr->ackNumber;

				//Congestion Window settings
				if(congWin < MSS && (LAR == lastSentSeqNum) && timeoutFlag ==0){
					//printf("\nBefore Slow start \n------------------------\nWindow size = %d packets", congWin/SEGMENTSIZE);
					congWin *= 2;
					printf("\nSlow start Phase \n------------------------\nExponential Increase Window size= %d packets", congWin/SEGMENTSIZE);
					receiveFlag = 0;
					slowStartPacketCount++;
				}
				else if(congWin < MSS && (LAR == lastSentSeqNum) && timeoutFlag == 1){
					congWin += SEGMENTSIZE;
					printf("\nCongestion Avoidance Phase \n------------------------\nAdditive Increase , Window size= %d packets", congWin/SEGMENTSIZE);
					receiveFlag = 0;
					addIncreasePacketCount++;
				}
				else if(congWin >= MSS && (LAR == lastSentSeqNum)){
					congWin = congWin/2;
					if(congWin < SEGMENTSIZE)
						congWin = SEGMENTSIZE;
					printf("\nCongestion Avoidance Phase\n------------------------\nMultiplicative Decrease Window Size = %d packets", congWin/SEGMENTSIZE);
					receiveFlag = 0;
					mulDecreasePacketCount++;
				}


				gettimeofday(&sCache[rudphdr->seqNumber].end, NULL);
				printf("\n-->Ack received for Packet%d!", rudphdr->ackNumber);
				printf("\nRound Trip Time: %.2lf",  ((double)sCache[rudphdr->seqNumber].end.tv_sec + (double)((double)sCache[rudphdr->seqNumber].end.tv_usec/ 1000000.0)) - ((double)sCache[rudphdr->seqNumber].start.tv_sec + (double)((double)sCache[rudphdr->seqNumber].start.tv_usec/ 1000000.0)));

				// Jacobson/ Karels Timeout calculation
				sCache[rudphdr->seqNumber].sampleRTT = ((double)sCache[rudphdr->seqNumber].end.tv_sec + (double)((double)sCache[rudphdr->seqNumber].end.tv_usec/ 1000000.0)) - ((double)sCache[rudphdr->seqNumber].start.tv_sec + (double)((double)sCache[rudphdr->seqNumber].start.tv_usec/ 1000000.0));
				sCache[rudphdr->seqNumber].difference = sCache[rudphdr->seqNumber].sampleRTT - sCache[rudphdr->seqNumber - 1].estimatedRTT;
				sCache[rudphdr->seqNumber].estimatedRTT = sCache[rudphdr->seqNumber - 1].estimatedRTT + (double)(DELTA) * sCache[rudphdr->seqNumber].difference;
				sCache[rudphdr->seqNumber].deviation = sCache[rudphdr->seqNumber - 1].deviation + (double)(DELTA) * (fabs((sCache[rudphdr->seqNumber].difference)) - sCache[rudphdr->seqNumber - 1].deviation);

				printf("\nSampleRTT: %0.2lf EstimatedRTT: %0.2lf Deviation: %0.2lf Difference: %0.2lf",sCache[rudphdr->seqNumber].sampleRTT,sCache[rudphdr->seqNumber].estimatedRTT,sCache[rudphdr->seqNumber].deviation,sCache[rudphdr->seqNumber].difference);
				timeOut.tv_sec = MU * sCache[rudphdr->seqNumber].estimatedRTT + PHI * sCache[rudphdr->seqNumber].deviation;
				timeOut.tv_usec = 0;

				if(timeOut.tv_sec < 2 || timeOut.tv_sec > 15)
					timeOut.tv_sec = 5;
				printf("\nTimeout value for next packet is %.2lf",(double)timeOut.tv_sec );
				printf("\nLFS: %d, LAR: %d",(LFS%4 + 1),(LAR%4 + 1));
				printf("\n---------------------------------------");

				if(LAR == LFS )
					LFS = LFS + 1;

			}

			else if(retVal == 0){
				fprintf(stderr,"\nTimer expired for Packet%d\n",rudphdr->seqNumber);
				printf("\nResending Packet%d\n",rudphdr->seqNumber);

				gettimeofday(&sCache[rudphdr->seqNumber].start, NULL);
				congWin = congWin/2;
				if(congWin < SEGMENTSIZE)
					congWin = SEGMENTSIZE;
				timeoutFlag = 1;
				retVal = 1;
				timeoutCount++;
				printf("\nCongestion Avoidance \n------------------------\nMultiplicative Decrease Window Size = %d packets", congWin/SEGMENTSIZE);
				goto timeout;

			}
		}
		if(rudphdr->seqNumber == lastSentSeqNum && isEOF == -1){
			listenFlag = 0;
			continue;
		}
	}
	printf("\n-------------------------------------------------------------");
	printf("\nTotal number of packets transmitted: %d",totalPacketCount + timeoutCount);
	printf("\nTotal number of packets transmitted in Slow Start phase: %d",totalPacketCount - addIncreasePacketCount -mulDecreasePacketCount);
	printf("\nTotal number of packets transmitted in Congestion Avoidance Phase(Additive increase): %d",addIncreasePacketCount);
	printf("\nTotal number of packets transmitted in Congestion Avoidance Phase(Multiplicative decrease): %d",mulDecreasePacketCount + timeoutCount);
	printf("\n");

	fclose(pFile);
	printf("\n");
	return 0;
}

