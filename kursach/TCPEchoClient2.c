#include <stdio.h>		/* for printf() and fprintf() */
#include <sys/socket.h>		/* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		/* for sockaddr_in and inet_addr() */
#include <stdlib.h>		/* for atoi() and exit() */
#include <string.h>		/* for memset() */
#include <unistd.h>		/* for close() */
#include <time.h>
#define MAX_MSG_SIZE 1024
#define MAXRECVSTRING 100	/* Longest string to receive */
#define FL 20
//#define SLEEP 5
#include "amessage.pb-c.h"


//#define RCVBUFSIZE 32         /* Size of receive buffer */
typedef struct DATA {
	char str[MAXRECVSTRING];
	int t;
	int len;
} Data;
Data data;

void DieWithError(char *errorMessage);	/* Error handling function */

int main(int argc, char *argv[])
{
	int sock;		/* Socket descriptor */
	struct sockaddr_in echoServAddr;	/* Echo server address */
	unsigned short echoServPort;	/* Echo server port */
	//char *servIP;                    /* Server IP address (dotted quad) */
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

		memset(&echoServAddr, 0, sizeof (echoServAddr));
		echoServAddr.sin_family = AF_INET;
		echoServAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		echoServAddr.sin_port = htons(echoServPort);
		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			DieWithError("socket() failed");

		if (connect
		    (sock, (struct sockaddr *) &echoServAddr,
		     sizeof (echoServAddr)) < 0)
			DieWithError("connect() failed");


		printf("Получено: ");

		AMessage *msg;

		uint8_t buf[MAX_MSG_SIZE];
		size_t cur_len = 0;
		size_t nread = 0;
		while ((nread =
			recv(sock, buf + cur_len, MAX_MSG_SIZE - cur_len,
			     0)) != 0) {
			cur_len += nread;
			if (cur_len == MAX_MSG_SIZE) {
				fprintf(stderr,
					"max message length exceeded\n");
				exit(1);
			}
		}


		size_t msg_len = cur_len;
		msg = amessage__unpack(NULL, msg_len, buf);
		if (msg == NULL) {
			fprintf(stderr,
				"error unpacking incoming message\n");
			exit(1);
		}
		sprintf(data.str, "%s", msg->strr);
		data.len = msg->len;
		data.t = msg->t;
		amessage__free_unpacked(msg, NULL);
		printf("%d %d %s\n", data.t, data.len, data.str);
		close(sock);
		raz = 0;
		sleep(data.t);
	}
	exit(0);
}
