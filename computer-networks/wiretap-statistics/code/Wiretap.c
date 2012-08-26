/* Program for reading Ethernet, IP, TCP/UDP headers from a tcpdump packet and printing out statistics */
  
/* Include files from standard libraries */

# include <stdio.h>
# include <stdlib.h>
# include <pcap.h>
# include <sys/types.h>
# include <net/ethernet.h>  /* Ethernet Header struct ether_header */
# include <netinet/ip.h>    /* IP Header struct ip */
# include <netinet/tcp.h>   /* TCP Header struct tcphdr */
# include <netinet/udp.h>   /* UDP Header struct udphdr */
# include <sys/socket.h>
# include <string.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <time.h>
# include <sys/time.h>

#define ETHERNET_PACKET_SIZE 14 /* Size of an ethernet packet is 14 bytes */

typedef struct {
	char addr[25];
	int flag;
	int count;
}ether_src_dest_addr;

typedef struct{
	u_int16_t pr_type;
	int flag;
	int count;
}network_layer_protocols;

typedef struct {
	char addr[15];
	int flag;
	int count;
}ip_src_dest_addr;

typedef struct{
	u_int16_t pr_type;
	int flag;
	int count;

}transport_layer_protocols;

typedef struct{
	int portnum;
	int flag;
	int count;

}transport_udp_tcp_ports;


typedef struct ether_header *Eth_Header;
typedef struct ip *Ip_Header;
typedef struct tcphdr *Tcp_Header; 
typedef struct udphdr *Udp_Header;
typedef struct pcap_pkthdr *Pcap_Header;

static ether_src_dest_addr ethArray[1024];
static network_layer_protocols netArray[1024];
static ip_src_dest_addr ipArray[1024];
static transport_layer_protocols transArray[1024];
static transport_udp_tcp_ports portsArray[1500];
int packetSize[1024];
int packetOverheadSize[1024];
static double timeStamp[1024];
time_t startTime ;
char *timeString;



static int ether_src_dest_cnt = 0;
static int ip_src_dest_cnt = 0;
static int packet_id = 1;
static int ethPacketCount = 0;
static int netProtoCount = 0;
static int netTotalCount = 0;
static int ipPacketCount = 0;
static int transProtoCount = 0;
static int transTotalCount = 0;
static int transPortCount = 0;
static int transPortTotalCount = 0;
static int packet_overhead_cnt = 0;
static int packet_cnt = 0;
static int time_cnt = 0;
static int checksumWrong = 0;
static int checksumOmitted = 0;
static int checksumCorrect = 0;
static int finCount = 0;
static int synCount = 0;
static int pshCount = 0;
static int rstCount = 0;
static int ackCount = 0;
static int urgCount = 0;
static int packetAvgSize = 0;
static int packetMinSize = 0;
static int packetMaxSize = 0;
static int packetSumSize = 0;
static int packetAvgOverHead = 0;
static int packetMinOverHead = 0;
static int packetMaxOverHead = 0;
static int packetSumOverHead = 0;





void packet_sniffer(u_char *args,const Pcap_Header pcap_pktheader, const u_char *rawPacket) ;

/* Structure variables for processing of the rawPacket received from pcap_loop */
Eth_Header ethPacket;
Ip_Header ipPacket;
Tcp_Header tcpPacket;
Udp_Header udpPacket;
u_int16_t *payload ;  /* Beyond TCP/UDP packet */

/* Variables for holding ip, tcp / udp packet sizes */
static u_short ipPacketSize;
static u_short tcpPacketSize;
static u_short udpPacketSize;
static u_short ethPacketSize;
static bpf_u_int32 pcapPacketSize;
u_int16_t udpHeaderLength = 0, Length = 0, data16 = 0, udpproto = 0x11, ipsrc1 = 0, ipsrc2 = 0, ipsrc3 = 0, ipsrc4 = 0, ipdst1 = 0, ipdst2 = 0, ipdst3 = 0, ipdst4 = 0,ipsrc11 = 0,ipsrc12 = 0,ipdst11 = 0,ipdst12 = 0 ;

unsigned long src = 0, dst = 0;




void packet_sniffer(u_char *args,const Pcap_Header pcap_pktheader, const u_char *rawPacket) {

	u_int32_t  payload_sum = 0, ipsrc = 0, ipdst = 0;
	long int sec = 0, microsec = 0;

	int i=0;	

	pcapPacketSize = pcap_pktheader->caplen;
	packetSize[packet_cnt] = pcapPacketSize;
//	printf("Total packet size :%d \n" , pcapPacketSize);
	packet_cnt++;


	sec = pcap_pktheader->ts.tv_sec;
	microsec = pcap_pktheader->ts.tv_usec;
	startTime = (time_t)sec ;
	timeString = ctime(&startTime);
	if(packet_id == 1) {
//	printf("\nElapsed time in secs %ld\n", sec);
//	printf("\nElapsed time in micro secs %ld\n", microsec);

	printf("\n ---------------------------------------------------------- \n");

	printf("\n The Start date and time of the Capture was at %s\n", timeString);
	}
	packet_id++;
	timeStamp[time_cnt] = (double)sec + (double)((double)microsec/ 1000000.0);
//	printf("\nElapsed time in total secs %lf\n", timeStamp[time_cnt]);
	time_cnt++;

	/* String value for the dest ethernet address */
	char ether_dhost_str[25] = "";

	/* String value for the source ethernet address */
	char ether_shost_str[25] = "", temp_str[20]= "";
	const char *delimiter = ":";
	
	char *srcIP, *dstIP;	
	u_int16_t srcPort = 0, dstPort = 0;
	
	u_short ethPacketSize = sizeof(struct ether_header);
	
	/* Pointer Typecasting. Pointing ethPacket to (Eth_Header)rawPacket */
	ethPacket = (Eth_Header)rawPacket; 
	
	for (i=0; i < ETH_ALEN; i++){ 

		sprintf(temp_str,"%x",ethPacket->ether_shost[i]);
		if(i < ETH_ALEN - 1) strncat(temp_str,delimiter,strlen(delimiter));
		strncat(ether_shost_str,temp_str,strlen(temp_str));
	}

//	printf("\n\n /* Packet %d */ \n ", packet_id++);	
//	printf(" \n/* Ethernet Header Statistics */ \n");	

	ethArray[ether_src_dest_cnt].flag = -1;
	ethArray[ether_src_dest_cnt].count = 1;
	strcpy(ethArray[ether_src_dest_cnt].addr,ether_shost_str);
//	printf("Concatenated Source ethernet address : %s \n",ethArray[ether_src_dest_cnt].addr );
	ether_src_dest_cnt++;

	for (i=0; i < ETH_ALEN; i++){ 
	
		sprintf(temp_str,"%x",ethPacket->ether_dhost[i]);
		if(i != ETH_ALEN - 1) strncat(temp_str,delimiter,strlen(delimiter));
		strncat(ether_dhost_str,temp_str,strlen(temp_str));
		
	}
	
	ethArray[ether_src_dest_cnt].flag = -1;
	ethArray[ether_src_dest_cnt].count = 1;
	
	strcpy(	ethArray[ether_src_dest_cnt].addr,ether_dhost_str);
//	printf("Concatenated Dest ethernet address : %s \n",ethArray[ether_src_dest_cnt].addr );
	ether_src_dest_cnt++;
	


	if(ntohs(ethPacket->ether_type) == ETHERTYPE_IP) {	


		netArray[netTotalCount].pr_type = ntohs(ethPacket->ether_type);
		netArray[netTotalCount].flag = -1;
		netArray[netTotalCount].count = 1;
		netTotalCount++;

		
		/*Ip Packet is located 14 bytes after Ethernet packet */
		ipPacket = (Ip_Header)(rawPacket + ETHERNET_PACKET_SIZE); 
	
		ipPacketSize = sizeof(struct ip);
		
		if(ipPacketSize < 20) {
			fprintf(stderr, "Invalid IP header length of %u bytes, has to be a minimum of 20 bytes \n", ipPacketSize);
			return;		
		}
		
//		printf(" /* IP Header Statistics */ \n");			
		
		srcIP  = (char *) inet_ntoa(ipPacket->ip_src);
		src = ntohl(ipPacket->ip_src.s_addr);
		ipsrc = (u_int32_t)src;
//		printf(" Source host IP Address : "); 
		ipsrc1 = (u_int16_t)((src>>24) & 0xff);
		ipsrc2 = (u_int16_t)((src>>16) & 0xff);
		ipsrc3 = (u_int16_t)((src>>8) & 0xff);
		ipsrc4 = (u_int16_t)(src & 0xff) ;
		ipsrc11 = (((ipsrc1<<8)& 0xff00) + (ipsrc2 & 0xff));
		ipsrc12 = (((ipsrc3<<8)& 0xff00) + (ipsrc4 & 0xff));
		
		
		strcpy(ipArray[ip_src_dest_cnt].addr, srcIP);
//		printf(": %s \n",ipArray[ip_src_dest_cnt].addr);
		ipArray[ip_src_dest_cnt].flag = -1;
		ipArray[ip_src_dest_cnt].count = 1;
		ip_src_dest_cnt++;

		//printf("%s %x %x %x %x %x %x %x",srcIP, ipsrc,ipsrc1,ipsrc2,ipsrc3,ipsrc4, ipsrc11, ipsrc12);

		//printf("\n Initial Payload\n");
		//printf("%x \n",	payload_sum);		
				
		//printf("\n Payload + ipsrc\n");
		payload_sum = payload_sum + ipsrc11 + ipsrc12;
		//printf("%x \n",	payload_sum);	
		//printf("\n");
		
		dstIP  = (char *) inet_ntoa(ipPacket->ip_dst);
		dst = ntohl(ipPacket->ip_dst.s_addr);
		ipdst = (u_int32_t)dst;
//		printf(" Destination host IP Address : "); 
		
		ipdst1 = (u_int16_t)((dst>>24) & 0xff);
		ipdst2 = (u_int16_t)((dst>>16) & 0xff);
		ipdst3 = (u_int16_t)((dst>>8) & 0xff);
		ipdst4 = (u_int16_t)(dst & 0xff) ;
		ipdst11 = (((ipdst1<<8)& 0xff00) + (ipdst2 & 0xff));
		ipdst12 = (((ipdst3<<8)& 0xff00) + (ipdst4 & 0xff));
		
		//printf("%s %x %x %x %x %x %x %x \n\n\n",dstIP, ipdst,ipdst1,ipdst2,ipdst3,ipdst4,ipdst11,ipdst12 );	
		//printf("\n Payload + ipdst\n");

		payload_sum = payload_sum + ipdst11 + ipdst12;
		//printf("%x \n",	payload_sum);		

		strcpy(ipArray[ip_src_dest_cnt].addr, dstIP);
//		printf(": %s \n",ipArray[ip_src_dest_cnt].addr);
		ipArray[ip_src_dest_cnt].flag = -1;
		ipArray[ip_src_dest_cnt].count = 1;
		ip_src_dest_cnt++;
		
		
	
		if(ipPacket->ip_p == 6) {

			/* Offset for TCP packet is obtained after adding the eth and ip packet sizes to the original offset */
			tcpPacket = (Tcp_Header)(rawPacket + ETHERNET_PACKET_SIZE + ipPacketSize) ;  
			tcpPacketSize = sizeof(struct tcphdr);
			
			packetOverheadSize[packet_overhead_cnt]	= (ethPacketSize + ipPacketSize + tcpPacketSize )*100/(u_short)( pcapPacketSize);


//			printf("\n packet overhead size is %u  \n", packetOverheadSize[packet_overhead_cnt]);
			packet_overhead_cnt++;
						
			transArray[transTotalCount].pr_type = ipPacket->ip_p;
			transArray[transTotalCount].flag = -1;
			transArray[transTotalCount].count = 1;
			
			transTotalCount++;


			if(tcpPacketSize < 20) {
				
				fprintf(stderr, "Invalid TCP header length of %u bytes, has to be a minimum of 160 bytes \n", tcpPacketSize);
				payload = (u_int16_t *)( rawPacket + ETHERNET_PACKET_SIZE + ipPacketSize + tcpPacketSize);  
				return ;		
			}

//		printf(" /* TCP Header Statistics */ \n");	

		srcPort  = tcpPacket->source;
//		printf(" Source Port : "); 
//		printf("%u",ntohs(srcPort));
//		printf("\n");
		
		 
//		printf(" fin:%u syn:%u rst:%u psh:%u ack:%u urg:%u ece:%u cwr:%u \n ",tcpPacket->fin,tcpPacket->syn,tcpPacket->rst,tcpPacket->psh,tcpPacket->ack,tcpPacket->urg,tcpPacket->res2,tcpPacket->res1); 
//		printf("\n");
		
		if(tcpPacket->fin == 1) finCount++;
		if(tcpPacket->syn == 1) synCount++;
		if(tcpPacket->rst == 1) rstCount++;;
		if(tcpPacket->psh == 1) pshCount++;
		if(tcpPacket->ack == 1) ackCount++;
		if(tcpPacket->urg == 1) urgCount++;

		portsArray[transPortTotalCount].portnum = ntohs(srcPort);
		portsArray[transPortTotalCount].flag = -1;
		portsArray[transPortTotalCount].count = 1;
		transPortTotalCount++;

		
		dstPort  = tcpPacket->dest;
//		printf(" Destination Port: "); 
//		printf("%u",ntohs(dstPort));	
//		printf("\n");
		
		portsArray[transPortTotalCount].portnum = ntohs(dstPort);
		portsArray[transPortTotalCount].flag = -1;
		portsArray[transPortTotalCount].count = 1;
		transPortTotalCount++;

		
		  
	
		}

		else if(ipPacket->ip_p == 17) {
			
			/* Offset for UDP packet is obtained after adding the eth and ip packet sizes to the original offset */
			udpPacket = (Udp_Header)(rawPacket + ETHERNET_PACKET_SIZE + ipPacketSize) ;  
			udpPacketSize = sizeof(struct udphdr);
			
			packetOverheadSize[packet_overhead_cnt]	= (ethPacketSize + ipPacketSize + udpPacketSize) *100/(u_short)( pcapPacketSize);
//			printf("\n packet overhead size is %u  \n", packetOverheadSize[packet_overhead_cnt]);
			packet_overhead_cnt++;
			
			transArray[transTotalCount].pr_type = ipPacket->ip_p;
			transArray[transTotalCount].flag = -1;
			transArray[transTotalCount].count = 1;
			
			transTotalCount++;

			if(udpPacketSize < 8) {

				fprintf(stderr, "Invalid UDP header length of %u bytes, has to be a minimum of 8 bytes \n", udpPacketSize);
				return ;		
			}
		
//		printf(" /* UDP Header Statistics */ \n");		
		srcPort  = udpPacket->source;
//		printf(" Source Port : "); 
//		printf("%u",ntohs(srcPort));
//		printf("\n");
		
		portsArray[transPortTotalCount].portnum = ntohs(srcPort);
		portsArray[transPortTotalCount].flag = -1;
		portsArray[transPortTotalCount].count = 1;
		transPortTotalCount++;
		
		dstPort  = udpPacket->dest;
//		printf(" Destination Port: "); 
//		printf("%u",ntohs(dstPort));	
//		printf("\n");
		
		portsArray[transPortTotalCount].portnum = ntohs(dstPort);
		portsArray[transPortTotalCount].flag = -1;
		portsArray[transPortTotalCount].count = 1;
		transPortTotalCount++;

		if(ntohs(udpPacket->check) == 0x0000) checksumOmitted++;

		udpHeaderLength = ntohs(udpPacket->len);
//		printf("UDP data for payload %x %x %x %x \n", ntohs(udpPacket->source),ntohs(udpPacket->dest),ntohs(udpPacket->len),ntohs(udpPacket->check) );
//		printf("\n Payload + udpdata\n");
		payload_sum = payload_sum + ntohs(udpPacket->source)+ ntohs(udpPacket->dest) + ntohs(udpPacket->len) + ntohs(udpPacket->check);
//		printf("UDP Header Length  is %u bytes\n\n", udpHeaderLength);
//		printf("%x\n",payload_sum);


		/*payload */

		payload = (u_int16_t *)( rawPacket + ETHERNET_PACKET_SIZE + ipPacketSize + udpPacketSize);  
		
		Length = udpHeaderLength;

		if(Length % 2 == 1) {
			Length = Length + 1;
		}
		//printf("%x payload[0]\n", ntohs(*(payload)));


		//printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP\n");

//		printf("\n Payload +data \n");		
		for(i = 0 ; i< ((Length - 8)/2) ; i = i+1){
			
			if(udpHeaderLength  % 2 == 1)
			if(i == (((Length-8)/2)-1)) *(payload+i) = (*(payload+i) & 0xff);
			data16 = ntohs(*(payload+i));
		//	printf("%x ->", data16);
			payload_sum = payload_sum + data16;
			//printf("\n\n\n");

		}
//			printf(" ( %x )", payload_sum);
		
//			printf("\n Payload + udppr + udplen\n");
			payload_sum = payload_sum + udpproto + ntohs(udpPacket->len);
//			printf(" ( %x )", payload_sum);
//			printf("\n\n\n");

		
//			printf("\n Payload >> 16\n");

		while(payload_sum >> 16){

					
				payload_sum = (payload_sum & 0xFFFF)+(payload_sum >> 16);

		}
//			printf("%x ->", payload_sum);
			//payload_sum = ~(payload_sum);

			//printf("\n\n Checksum: %x \n", (u_int16_t)payload_sum);
			
			if((u_int16_t)payload_sum == 0xffff) checksumCorrect++;
			else if((u_int16_t)payload_sum != 0xffff) checksumWrong++;

			
		//printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP\n");			


			
		}
		
		else {
//			printf(" \n\nTransport Layer Protocol Number -> %#x \n",ipPacket->ip_p );		
			transArray[transTotalCount].pr_type = ipPacket->ip_p;
			transArray[transTotalCount].flag = -1;
			transArray[transTotalCount].count = 1;
			
			transTotalCount++;

		}

	}
		else if(ntohs(ethPacket->ether_type) != ETHERTYPE_IP) {
//			printf(" \n\n Network Layer Protocol Number -> %#x \n",ntohs(ethPacket->ether_type) );		
			netArray[netTotalCount].pr_type = ntohs(ethPacket->ether_type);
			netArray[netTotalCount].flag = -1;
			netArray[netTotalCount].count = 1;
			
			netTotalCount++;

		
		}	
	
}

int main(int argc, char **argv) {
	
	pcap_t *pcapHandler = NULL;


	char *file1 ;
	file1 = argv[1];
	char errorBuffer[PCAP_ERRBUF_SIZE]; /*errorBuffer is used to hold the error incase of failure */
	int dataLinkType = 0;
	int i=0,j=0;

	/* Read data from traceroute.pcap and wget.pcap using pcap_open_offline*/
	pcapHandler =  pcap_open_offline(file1,errorBuffer);

	if(pcapHandler == NULL) {

		fprintf(stderr,"Couldn't open the file : %s \n", errorBuffer);
		return 2;		
	}	
	
	/*Check whether the data has been captured from Ethernet */
	dataLinkType =	pcap_datalink(pcapHandler);	

	if( dataLinkType == DLT_EN10MB) 

		printf("\n\n Datalink type is Ethernet : %d \n\n", dataLinkType); 

	/* Call pcap_loop to loop through the packets and strip individual Ethernet, IP, TCP/UDP headers */
	if(pcap_loop(pcapHandler, -1, (void *)packet_sniffer, NULL) == -1) {

		fprintf(stderr,"Error processing the packet at pcap_loop() \n");
		return 2;
	}
	
//	printf("\n\nLength of ether_src_dest_array is : %d ", ether_src_dest_cnt);

//	for(i =0; i< ether_src_dest_cnt; i++)
//	printf("%s : %d\n",ethArray[i].addr,ethArray[i].flag); 



	printf("\n ---------------------------------------------------------- \n");

	
	/* printing unique Ethernet addresses */
	for(i =0; i< ether_src_dest_cnt; i++) {
		if(ethArray[i].flag == -1) {
			  for(j = i+1; j< ether_src_dest_cnt; j++){
				if(ethArray[j].flag == -1) {
					if( strcmp(ethArray[i].addr,ethArray[j].addr) != 0) continue;

					else if( strcmp(ethArray[i].addr,ethArray[j].addr) == 0) {
						ethArray[i].flag = 1 ;
						ethArray[j].flag = 0; 
						ethArray[i].count++;
						ethArray[j].count = 0;
					}			
				  }
			  }			
		}
		else if(ethArray[i].flag != -1) continue;
		
	}



	for(i =0; i< ether_src_dest_cnt; i++){
		if(ethArray[i].flag == 1 || ethArray[i].count == 1 ){
			ethPacketCount++;
			printf("Ethernet Address: %s and count: %d \n",ethArray[i].addr,ethArray[i].count);
						 
		}
	}
	
		printf("\nNumber of Unique Ethernet Addresses = %d \n", ethPacketCount);

	printf("\n ---------------------------------------------------------- \n");

	/* printing unique network layer protocols and packets using them */


//	printf("\n\nLength of netArray is : %d \n", netTotalCount);

//	for(i =0; i< netTotalCount; i++)
//	printf("0x%d : %d\n",netArray[i].pr_type,netArray[i].flag); 
	
	/* printing unique Network layer protocols addresses */

	for(i =0; i< netTotalCount; i++) {
		if(netArray[i].flag == -1) {
			  for(j = i+1; j< netTotalCount; j++){
				if(netArray[j].flag == -1) {
					if( netArray[i].pr_type != netArray[j].pr_type) continue;

					else if( netArray[i].pr_type == netArray[j].pr_type) {
						netArray[i].flag = 1 ;
						netArray[j].flag = 0; 
						netArray[i].count++;
						netArray[j].count = 0;
					}			
			  	}	
			  }
		  }
		else if(netArray[i].flag != -1) continue;
		
	}

	

	for(i =0; i< netTotalCount; i++){
		if(netArray[i].flag == 1 || netArray[i].count == 1 ){
			netProtoCount++;
			if(netArray[i].pr_type == 0x8)
				printf("Network Layer Protocol: IP and count: %d\n ",netArray[i].count);
			else{
			printf("Network Layer Protocol: 0x%x count: %d\n ",netArray[i].pr_type,netArray[i].count);
			}			 
		}
	}
	
		printf("\nNumber of Unique Network layer protocols= %d \n", netProtoCount);

	printf("\n ---------------------------------------------------------- \n");

	/* printing unique ip addresses and packets containing them */

//	printf("\n\nLength of ip_src_dest_array is : %d ", ip_src_dest_cnt);

//	for(i =0; i< ip_src_dest_cnt; i++)
//	printf("(%s : %d %d)",ipArray[i].addr,ipArray[i].flag,ipArray[i].count); 
//	printf("\n\n");


	/* printing unique IP addresses */
	for(i =0; i< ip_src_dest_cnt; i++) {
		if(ipArray[i].flag == -1) {
			  for(j = i+1; j< ip_src_dest_cnt; j++){
				if(ipArray[j].flag == -1) {
					if( strcmp(ipArray[i].addr,ipArray[j].addr) != 0) continue;

					else if( strcmp(ipArray[i].addr,ipArray[j].addr) == 0) {
						ipArray[i].flag = 1 ;
						ipArray[j].flag = 0; 
						ipArray[i].count++;
						ipArray[j].count = 0;

					}			
				  }
			  }			
		}
		else if(ipArray[i].flag != -1) continue;
		
	}


	for(i =0; i< ip_src_dest_cnt; i++){
		if(ipArray[i].flag == 1 || ipArray[i].count == 1 ){
			ipPacketCount++;
			printf("IP Address: %s and count:%d \n",ipArray[i].addr,ipArray[i].count);
						 
		}
	}

//	for(i =0; i< ip_src_dest_cnt; i++)
//	printf("(%s : %d %d)\n",ipArray[i].addr,ipArray[i].flag,ipArray[i].count); 

	printf("\nNumber of Unique IP Addresses= %d \n", ipPacketCount);

	printf("\n ---------------------------------------------------------- \n");


	/* printing unique transport layer protocols and packets using them */


//	printf("\n\nLength of transArray is : %d \n", transTotalCount);

//	for(i =0; i< transTotalCount; i++)
//	printf("0x%d : %d\n",transArray[i].pr_type,transArray[i].flag); 
	/* printing unique transport layer protocols addresses */

	for(i =0; i< transTotalCount; i++) {
		if(transArray[i].flag == -1) {
			  for(j = i+1; j< transTotalCount; j++){
				if(transArray[j].flag == -1) {
					if( transArray[i].pr_type != transArray[j].pr_type) continue;

					else if( transArray[i].pr_type == transArray[j].pr_type) {
						transArray[i].flag = 1 ;
						transArray[j].flag = 0; 
						transArray[i].count++;
						transArray[j].count = 0;
					}			
			  	}	
			  }
		  }
		else if(transArray[i].flag != -1) continue;
		
	}

		

	for(i =0; i< transTotalCount; i++){
		if(transArray[i].flag == 1 || transArray[i].count == 1){
			transProtoCount++;

			if(transArray[i].pr_type == 0x6)
				printf("Transport Layer Protocol: TCP and count: %d\n ",transArray[i].count);

			else if(transArray[i].pr_type == 0x11)
				printf("Transport Layer Protocol: UDP and count: %d\n ",transArray[i].count);

			else{
			printf("Transport Layer Protocol : 0x%x count:%d \n",transArray[i].pr_type,transArray[i].count);
			}			 
		}
	}
	

	printf("\nNumber of Unique Transport layer protocols = %d \n", transProtoCount);

	printf("\n ---------------------------------------------------------- \n");

	/* printing unique TCP and UDP ports and packets using them */


//	printf("\n\nLength of portsArray is : %d \n", transPortTotalCount);

//	for(i =0; i< transPortTotalCount; i++)
//	printf("%d : %d\n",portsArray[i].portnum,portsArray[i].flag); 

	/* printing unique TCP and UDP layer port numbers */

	for(i =0; i< transPortTotalCount; i++) {
		if(portsArray[i].flag == -1) {
			  for(j = i+1; j< transPortTotalCount; j++){
				if(portsArray[j].flag == -1) {
					if( portsArray[i].portnum != portsArray[j].portnum) continue;

					else if( portsArray[i].portnum == portsArray[j].portnum) {
						portsArray[i].flag = 1 ;
						portsArray[j].flag = 0; 
						portsArray[i].count++;
						portsArray[j].count = 0;
					}			
			  	}	
			  }
		  }
		else if(portsArray[i].flag != -1) continue;
		
	}

	for(i =0; i< transPortTotalCount; i++){
		if(portsArray[i].flag == 1 || portsArray[i].count == 1){
			transPortCount++;
			printf("Port Number: %d and count:%d \n",portsArray[i].portnum,portsArray[i].count);
						 
		}
	}
	printf("\nNumber of Unique TCP and UDP port numbers = %d \n", transPortCount);

	

	printf("\n ---------------------------------------------------------- \n");
		
	/* printing the flags and no.of packets containing each flag */

	printf("\n\nNo.of packets containing FIN flag: %d \n", finCount);
	printf("No.of packets containing SYN flag: %d \n", synCount);
	printf("No.of packets containing RST flag: %d \n", rstCount);
	printf("No.of packets containing PSH flag: %d \n", pshCount);
	printf("No.of packets containing ACK flag: %d \n", ackCount);
	printf("No.of packets containing URG flag: %d \n", urgCount);

	printf("\n ---------------------------------------------------------- \n");

	/* Average, Minimum and Maximum packet sizes */
	packetMinSize = packetSize[0] 	;
//	printf("\n packetMinSize :%d \n", packetMinSize);
	packetMaxSize = packetSize[0]   ; 

//	for(i =0; i< packet_cnt; i++)
//		printf("%d ->",packetSize[i]);


	for(i =0; i< packet_cnt; i++){

		if(packetSize[i] < packetMinSize) packetMinSize = packetSize[i];
		if(packetSize[i] > packetMaxSize) packetMaxSize = packetSize[i];
		packetSumSize += packetSize[i];
		
	}
		packetAvgSize = packetSumSize / packet_cnt;

	printf("\n\nAverage Packet Size: %d bytes \n", packetAvgSize); 
	printf("Minimum Packet Size: %d bytes\n", packetMinSize); 
	printf("Maximum Packet Size: %d bytes\n", packetMaxSize); 


	printf("\n ---------------------------------------------------------- \n");

	/* Average, Minimum and Maximum packet overheads */
 
	packetMinOverHead = packetOverheadSize[0]   ;
//	printf("\n packetMinOverHead :%d \n", packetMinOverHead);
	packetMaxOverHead = packetOverheadSize[0]   ; 

//	for(i =0; i< packet_overhead_cnt; i++)
//		printf("(%d) ",packetOverheadSize[i]);


	for(i =0; i< packet_overhead_cnt; i++){

		if(packetOverheadSize[i] < packetMinOverHead) packetMinOverHead = packetOverheadSize[i];
			//printf("\n packetMinOverHead :%d \n", packetMinOverHead);
		if(packetOverheadSize[i] > packetMaxOverHead) packetMaxOverHead = packetOverheadSize[i];

		packetSumOverHead += packetOverheadSize[i];
		
	}
		packetAvgOverHead = packetSumOverHead / packet_overhead_cnt;

	printf("\n\nAverage Packet Overhead: %d \n", packetAvgOverHead); 
	printf("Minimum Packet Overhead: %d \n", packetMinOverHead); 
	printf("Maximum Packet Overhead: %d \n", packetMaxOverHead); 

	printf("\n ---------------------------------------------------------- \n");

	printf("\nTotal duration of the capture is %lf secs\n", timeStamp[time_cnt - 1] - timeStamp[0]);
	printf("\nTotal number of packets in the capture is %d \n", (packet_id-1));

	printf("\n ---------------------------------------------------------- \n");


	printf("\n UDP packets with correct checksum: %d", checksumCorrect);
	printf("\n UDP packets with wrong checksum: %d", checksumWrong);
	printf("\n UDP packets with omitted checksum: %d", checksumOmitted);

	/* Close the pcap Handler */
	pcap_close(pcapHandler);

	return 0;

}
 
