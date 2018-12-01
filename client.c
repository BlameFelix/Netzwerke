#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUF_LEN 128

// Something unexpected happened. Report error and terminate.
void sysErr( char *msg, int exitCode )
{
	fprintf( stderr,"%s\n\t%s\n", msg, strerror( errno ) );
	exit( exitCode );
}

// Called with wrong arguments.
void usage( char *argv0 )
{
	printf( "usage : %s server-ip portnumber\n", argv0 );
	exit( 0 );
}

int main(int argc, char **argv)
{
	//checking if a server-ip and a port have been supplied
	if(argc<3){
		usage(argv[0]);
	}

	//initializing variables
	struct sockaddr_in serv_addr;
	char sendBuf[BUF_LEN];
	//unsigned int len;

	//creating TCP based socket
	int sockfd = 0;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1){
		sysErr("Client Fault: socket", -1);
	}

	//setting up socketadress of the client
	memset( &serv_addr, 0, sizeof(serv_addr) );
	//converting adress and putting it into the struct
	if ( ( serv_addr.sin_addr.s_addr = inet_addr( argv[ 1 ] ) ) == INADDR_NONE ) {
		sysErr( "Client Fault: NOT_AN_IP", -2 );
	}
	//IPv4 based connection
	serv_addr.sin_family = AF_INET;
	//setting port number converted to network byte order
	serv_addr.sin_port = htons( (u_short)atoi( argv[ 2 ] ) );

	//Connect to server
	if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		sysErr("Client Fault: connect", -3);
	}
	//user input
	printf("Enter String: ");
	fgets(sendBuf,BUF_LEN-1,stdin);

	//send message
	if((send(sockfd, sendBuf, strlen(sendBuf),0))==-1){
		sysErr("Client fault: send message", -4);
	}
	//get thex responde
	char recvBuf[BUF_LEN];
	if(/*len = */recv(sockfd, recvBuf, BUF_LEN, 0)==-1){
		sysErr("Client Fault: recive message", -5);
	}
	//print the responde
	write(STDOUT_FILENO, recvBuf, strlen(recvBuf));

	close(sockfd);
	return 0;
}/*
//----------------------old code-------------------------------------------------------------------------eof------------------------------------------------
	int sockfd;
	socklen_t addrLen = sizeof(struct sockaddr_in);
	struct sockaddr_in serv_addr;
	char sendBuff[BUF_LEN];

	sendBuff[BUF_LEN-1] = 0;

	// Check for right number of arguments
	if ( argc < 3 ) usage( argv[0] );

	// Fill struct addr with IP addr, port and protocol family
	memset( &serv_addr, 0, addrLen );
	if ( ( serv_addr.sin_addr.s_addr = inet_addr( argv[ 1 ] ) ) == INADDR_NONE ) {
		sysErr( "Client Fault: NOT_AN_IP", -1 );
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons( (u_short)atoi( argv[ 2 ] ) );

	// Create new socket based on the values from
	if ( ( sockfd = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 ) {
		sysErr( "Client Fault: SOCKET", -2 );
	}

	printf("Enter String: ");
	fgets(sendBuff,BUF_LEN-1,stdin);
	printf("Sending     : %s\n", sendBuff);
	if ( sendto( sockfd, sendBuff, strlen(sendBuff), 0, (struct sockaddr *) &serv_addr, addrLen ) ==  -1 ) {
			sysErr( "Client Fault: SENDTO", -4 );
	}

	close( sockfd );
	return 0;
}*/
