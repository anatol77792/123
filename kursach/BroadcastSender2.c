#include <stdio.h>		/* for printf() and fprintf() */
#include <sys/socket.h>		/* for socket() and bind() */
#include <arpa/inet.h>		/* for sockaddr_in */
#include <stdlib.h>		/* for atoi() and exit() */
#include <string.h>		/* for memset() */
#include <unistd.h>		/* for close() */
#include "TCPEchoServer.h"	/* TCP echo server includes */
void DieWithError(char *errorMessage);	/* External error handling function */

void BroadcastSender2(char *broadcastIP, unsigned short broadcastPort)
{
	int sock;		/* Socket */
	struct sockaddr_in broadcastAddr[150];	/* Broadcast address */
	// char *broadcastIP;                /* IP broadcast address */
	//unsigned short broadcastPort;     /* Server port */
	char *sendString;	/* String to broadcast */
	int broadcastPermission;	/* Socket opt to set permission to broadcast */
	unsigned int sendStringLen;	/* Length of string to broadcast */
	printf("%s\n", broadcastIP);
	printf("%d\n", broadcastPort);

	//if (argc < 4)                     /* Test for correct number of parameters */
	// {
	//    fprintf(stderr,"Usage:  %s <IP Address> <Port> <Send String>\n", argv[0]);
	//    exit(1);
	//  }

	// broadcastIP = argv[1];            /* First arg:  broadcast IP address */ 
	// broadcastPort = atoi(argv[2]);    /* Second arg:  broadcast port */
	sendString = "20";	/* Third arg:  string to broadcast */

	/* Create socket for sending/receiving datagrams */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		DieWithError("socket() failed");

	/* Set socket to allow broadcast */
	broadcastPermission = 1;
	if (setsockopt
	    (sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,
	     sizeof (broadcastPermission)) < 0)
		DieWithError("setsockopt() failed");

	/* Construct local address structure */
	for (int i = 0; i < 100; i++) {
		memset(&broadcastAddr[i], 0, sizeof (broadcastAddr[i]));	/* Zero out structure */
		broadcastAddr[i].sin_family = AF_INET;	/* Internet address family */
		broadcastAddr[i].sin_addr.s_addr = inet_addr(broadcastIP);	/* Broadcast IP address */
		broadcastAddr[i].sin_port = htons(broadcastPort + i);	/* Broadcast port */
	}
	sendStringLen = strlen(sendString);	/* Find length of sendString */

	for (;;) {		/* Run forever */
		for (int i = 0; i < 100; i++) {
			if (och > 1) {
				/* Broadcast sendString in datagram to clients every 3 seconds */
				sendto(sock, sendString, sendStringLen, 0,
				       (struct sockaddr *)
				       &broadcastAddr[i],
				       sizeof (broadcastAddr[i]));
			} else {
				sleep(3);	/* Avoids flooding the network */


			}
		}
	}
	/* NOT REACHED */
}
