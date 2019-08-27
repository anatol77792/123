#include <stdio.h>		/* for printf() and fprintf() */
#include <sys/socket.h>		/* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		/* for sockaddr_in and inet_addr() */
#include <stdlib.h>		/* for atoi() and exit() */
#include <string.h>		/* for memset() */
#include <unistd.h>		/* for close() */
#define MAXRECVSTRING 255	/* Longest string to receive */
#define FL 20
#define RCVBUFSIZE 32		/* Size of receive buffer */

void DieWithError(char *errorMessage);	/* Error handling function */

int main(int argc, char *argv[])
{
	int sock;		/* Socket descriptor */
	struct sockaddr_in echoServAddr;	/* Echo server address */
	unsigned short echoServPort;	/* Echo server port */
	//char *servIP;                    /* Server IP address (dotted quad) */
	char *echoString;	/* String to send to echo server */
	char echoBuffer[RCVBUFSIZE];	/* Buffer for echo string */
	unsigned int echoStringLen;	/* Length of string to echo */
	int bytesRcvd, totalBytesRcvd;	/* Bytes read in single recv() 
					   and total bytes read */

	int sock1;		/* Socket */
	struct sockaddr_in broadcastAddr;	/* Broadcast Address */
	unsigned short broadcastPort;	/* Port */
	char recvString[MAXRECVSTRING + 1];	/* Buffer for received string */
	int recvStringLen;	/* Length of received string */
	int raz;
	int tt;
	if ((argc < 3)) {	/* Test for correct number of arguments */
		fprintf(stderr,
			"Usage: %s <broadcast Port> [<Echo Port>]\n",
			argv[0]);
		exit(1);
	}
	//servIP = argv[1];             /* First arg: server IP address (dotted quad) */
	echoString = "40               ";	/* Second arg: string to echo */

	echoServPort = atoi(argv[2]);	/* Use given port, if any */
	/*  if (argc == 4)

	   else
	   echoServPort = 7;  */

	for (;;) {
		while (raz != FL) {

			broadcastPort = atoi(argv[1]);	// First arg: broadcast port 

			// Create a best-effort datagram socket using UDP 
			if ((sock1 =
			     socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
				DieWithError("socket() failed");

			// Construct bind structure 
			memset(&broadcastAddr, 0, sizeof (broadcastAddr));	// Zero out structure 
			broadcastAddr.sin_family = AF_INET;	// Internet address family 
			broadcastAddr.sin_addr.s_addr = inet_addr("127.255.255.255");	// Any incoming interface 
			broadcastAddr.sin_port = htons(broadcastPort);	// Broadcast port 

			// Bind to the broadcast port 
			if (bind
			    (sock1, (struct sockaddr *) &broadcastAddr,
			     sizeof (broadcastAddr)) < 0)
				DieWithError("bind() failed");

			//Receive a single datagram from the server
			if ((recvStringLen =
			     recvfrom(sock1, recvString, MAXRECVSTRING, 0,
				      NULL, 0)) < 0)
				DieWithError("recvfrom() failed");

			recvString[recvStringLen] = '\0';
			//  printf("Received: %s\n", recvString);    // Print the received string 
			raz = atoi(recvString);
		}
		close(sock1);
		//  exit(0);

		memset(&echoServAddr, 0, sizeof (echoServAddr));	/* Zero out structure */
		echoServAddr.sin_family = AF_INET;	/* Internet address family */
		echoServAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);	/* Server IP address */
		echoServAddr.sin_port = htons(echoServPort);	/* Server port */
		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			DieWithError("socket() failed");
		/* Establish the connection to the echo server */
		if (connect
		    (sock, (struct sockaddr *) &echoServAddr,
		     sizeof (echoServAddr)) < 0)
			DieWithError("connect() failed");

		echoStringLen = strlen(echoString);	/* Determine input length */

		/* Send the string to the server */
		if (send(sock, echoString, echoStringLen, 0) !=
		    echoStringLen)
			DieWithError
			    ("send() sent a different number of bytes than expected");

		/* Receive the same string back from the server */
		totalBytesRcvd = 0;
		printf("Получено: ");	/* Setup to print the echoed string */
		while (totalBytesRcvd < echoStringLen) {
			/* Receive up to the buffer size (minus 1 to leave space for
			   a null terminator) bytes from the sender */
			if ((bytesRcvd =
			     recv(sock, echoBuffer, RCVBUFSIZE - 1,
				  0)) <= 0)
				DieWithError
				    ("recv() failed or connection closed prematurely");
			totalBytesRcvd += bytesRcvd;	/* Keep tally of total bytes */
			echoBuffer[bytesRcvd] = '\0';	/* Terminate the string! */
			printf("%s", echoBuffer);	/* Print the echo buffer */
			char *red = strtok(echoBuffer, " ");

			tt = atoi(red);


		}

		printf("\n");	/* Print a final linefeed */
		close(sock);
		raz = 0;
		sleep(tt);
	}
	exit(0);
}
