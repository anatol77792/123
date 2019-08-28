#include <stdio.h>		/* for printf() and fprintf() */
#include <sys/socket.h>		/* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		/* for sockaddr_in and inet_addr() */
#include <stdlib.h>		/* for atoi() and exit() */
#include <string.h>		/* for memset() */
#include <unistd.h>		/* for close() */
#include <time.h>
#define MAXRECVSTRING 100	/* Longest string to receive */
#define FL 10
#define SLEEP 5
#include "amessage.pb-c.h"

#define RCVBUFSIZE 32		/* Size of receive buffer */
typedef struct DATA {
	char str[MAXRECVSTRING];
	int t;
	int len;
} Data;
Data data;

void DieWithError(char *errorMessage);	/* Error handling function */

int main(int argc, char *argv[])
{
	AMessage msg = AMESSAGE__INIT;	// AMessage
	void *buf;		// Buffer to store serialized data
	unsigned lenn;		// Length of serialized data
	int sock;		/* Socket descriptor */
	struct sockaddr_in echoServAddr;	/* Echo server address */
	unsigned short echoServPort;	/* Echo server port */
	//char *servIP;                    /* Server IP address (dotted quad) */
	char echoString[MAXRECVSTRING];	/* String to send to echo server */


	int sock1;		/* Socket */
	struct sockaddr_in broadcastAddr;	/* Broadcast Address */
	unsigned short broadcastPort;	/* Port */
	char recvString[MAXRECVSTRING + 1];	/* Buffer for received string */
	int recvStringLen;	/* Length of received string */
	int raz;
	if ((argc < 3)) {	/* Test for correct number of arguments */
		fprintf(stderr,
			"Usage: %s <broadcast Port> [<Echo Port>]\n",
			argv[0]);
		exit(1);
	}
	echoServPort = atoi(argv[2]);
	//servIP = argv[1];             /* First arg: server IP address (dotted quad) */

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
//    printf("Received: %s\n", recvString);    // Print the received string 
			raz = atoi(recvString);
		}
		close(sock1);

		int stime;
		long ltime = time(NULL);
		stime = (unsigned int) ltime / 2;
		srand(stime);
		int a = 4 + rand() % 5;
		char str[a];
		for (int i = 0; i < a; i++) {
			str[i] = (int) ('a' + rand() % 25);
			str[a] = 0;

		}

		int t = rand() % SLEEP;
		int len = strlen(str);
		sprintf(echoString, "%d %d %s", t, len, str);
		data.len = len;
		sprintf(data.str, "%s", str);
		data.t = t;
		msg.t = t;
		msg.len = len;
		msg.strr = str;
		lenn = amessage__get_packed_size(&msg);
		buf = malloc(lenn);
		amessage__pack(&msg, buf);

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
		//echoStringLen = sizeof(data); /* Determine input length */

		/* Send the string to the server */
		if (send(sock, buf, lenn, 0) != lenn)
			DieWithError
			    ("send() sent a different number of bytes than expected");
		printf("Отправлено %d %d %s\n", data.t, data.len,
		       data.str);
		close(sock);
		raz = 0;
		free(buf);
		sleep(4);
	}
	exit(0);
}
