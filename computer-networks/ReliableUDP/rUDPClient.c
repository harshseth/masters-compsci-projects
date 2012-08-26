/*
 * rUDPClient.c
 *
 *  Created on: Nov 29, 2009
 *      Author: deepakkoni
 */

/* Simple UDP echo client - tries to send everything read from stdin
   as a single datagram (MAX 1MB)*/

#include <stdio.h>      /* standard C i/o facilities */
#include <stdlib.h>     /* needed for atoi() */
#include <unistd.h>  	/* defines STDIN_FILENO, system calls,etc */
#include <sys/types.h>  /* system data type definitions */
#include <sys/socket.h> /* socket specific definitions */
#include <netinet/in.h> /* INET constants and stuff */
#include <arpa/inet.h>  /* IP address conversion stuff */
#include <netdb.h>		/* gethostbyname */
#include <string.h>


#define MAXBUF 10*1024


int get_stdin( char *buf, int maxlen ) {

	int i=0;
	int n;

	while ( (n=read(STDIN_FILENO,buf+i,maxlen-i)) > 0 ) {
		i+=n;
		if (i==maxlen) break;
	}

	if (n!=0) {
		fprintf(stderr,"Error reading stdin");
		exit(1);
	}

	return(i);
}



int main( int argc, char **argv ) {

	int socketDescriptor;
	struct sockaddr_in *serverSockAddr = (struct sockaddr_in *)calloc(1,sizeof(struct sockaddr_in));
	struct hostent *hp;
	char cBuffer[MAXBUF];
	int cBufferLen;
	int n_sent;
	int n_read;


	if (argc!=3) {
		printf("Usage: %s <server name> <port number>\n",argv[0]);
		exit(0);
	}


	if ((socketDescriptor = socket( PF_INET, SOCK_DGRAM, 0 )) < 0){
		printf("Problem creating socket\n");
		exit(1);
	}

	serverSockAddr->sin_family = AF_INET;

	if ((hp = gethostbyname(argv[1]))==0) {
		printf("Invalid or unknown host\n");
		exit(1);
	}


	memcpy(&serverSockAddr->sin_addr.s_addr,  hp->h_addr_list[0], hp->h_length);

	serverSockAddr->sin_port = htons(atoi(argv[2]));

	cBufferLen = get_stdin(cBuffer,10);
	printf("Got %d bytes from stdin - sending...\n",cBufferLen);

	n_sent = sendto(socketDescriptor,cBuffer,cBufferLen,0,(struct sockaddr*)serverSockAddr,sizeof(*serverSockAddr));

	if (n_sent<0) {
		fprintf(stderr,"Problem sending data");
		exit(1);
	}

	if (n_sent!=cBufferLen) {
		printf("Sendto sent %d bytes\n",n_sent);
	}

	n_read = recvfrom(socketDescriptor,cBuffer,MAXBUF,0,NULL,NULL);
	if (n_read<0) {
		fprintf(stderr,"Problem in recvfrom");
		exit(1);
	}

	printf("Got back %d bytes\n",n_read);

	if (write(STDOUT_FILENO,cBuffer,n_read) < 0) {
		fprintf(stderr,"Problem writing to stdout");
		exit(1);
	}

	return(0);
}







