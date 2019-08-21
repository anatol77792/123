#include <stdio.h>		/* for printf() and fprintf() */
#include <sys/socket.h>		/* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		/* for sockaddr_in and inet_addr() */
#include <stdlib.h>		/* for atoi() and exit() */
#include <string.h>		/* for memset() */
#include <unistd.h>		/* for close() */

#define RCVBUFSIZE 1024		/* Size of receive buffer */

void DieWithError(char *errorMessage);	/* Error handling function */

int main(int argc, char *argv[])
{
	int sock;		/* Socket descriptor */
	struct sockaddr_in echoServAddr;	/* Echo server address */
	unsigned short echoServPort;	/* Echo server port */
	// char *servIP;                    /* Server IP address (dotted quad) */
	// char *echoString;                /* String to send to echo server */
	char echoBuffer[RCVBUFSIZE];	/* Buffer for echo string */
	// unsigned int echoStringLen;      /* Length of string to echo */
	int bytesRcvd, totalBytesRcvd;	/* Bytes read in single recv() 
					   and total bytes read */
	const int BUFF_SIZE = 1024;
	char echoString[BUFF_SIZE];
	char buff11[BUFF_SIZE];
	int x;
	int y;
	int kolj;

	if ((argc < 3) || (argc > 4)) {	/* Test for correct number of arguments */
		fprintf(stderr, "Usage: %s <x> <y> [<Echo Port>]\n", argv[0]);
		exit(1);
	}

	x = atoi(argv[1]);	/* First arg: server IP address (dotted quad) */


	y = atoi(argv[2]);	/* Second arg: string to echo */

	if (argc == 4)
		echoServPort = atoi(argv[3]);	/* Use given port, if any */
	else
		echoServPort = 7;	/* 7 is the well-known port for the echo service */

	/* Create a reliable, stream socket using TCP */

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket() failed");

	/* Construct the server address structure */
	memset(&echoServAddr, 0, sizeof (echoServAddr));	/* Zero out structure */
	echoServAddr.sin_family = AF_INET;	/* Internet address family */
	echoServAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);	/* Server IP address */
	echoServAddr.sin_port = htons(echoServPort);	/* Server port */

	/* Establish the connection to the echo server */

	if (connect
	    (sock, (struct sockaddr *) &echoServAddr,
	     sizeof (echoServAddr)) < 0) {
		DieWithError("connect() failed");
	}
	for (;;) {
		int count = 0;
		printf
		    ("Введите направление движения:");
		scanf("%d", &count);
		printf("n=%i\n", count);
		getchar();
		if (count == 1) {
			x = x - 1;
			y = y - 1;
		} else {
			if (count == 2) {
				x = x;
				y = y - 1;
			} else {
				if (count == 3) {
					x = x + 1;
					y = y - 1;
				} else {
					if (count == 4) {
						x = x - 1;
						y = y;
					} else {
						if (count == 6) {
							x = x + 1;
							y = y;
						} else {
							if (count == 7) {
								x = x - 1;
								y = y + 1;
							} else {
								if (count == 8) {
									x = x;
									y = y +
									    1;
								} else {
									if (count == 9) {
										x = x + 1;
										y = y + 1;
									} else {
										x = x;
										y = y;
									}
								}
							}
						}
					}
				}
			}
		}

		sprintf(echoString, "%d", x);
		strcat(echoString, " ");

		sprintf(buff11, "%d", y);
		strcat(echoString, buff11);
		strcat(echoString, "     ");
		printf("%s\n", echoString);
		//  echoStringLen = strlen(echoString);          /* Determine input length */

		/* Send the string to the server */
		send(sock, echoString, sizeof (echoString), 0);

		/* Receive the same string back from the server */
		//totalBytesRcvd = 0;
		printf("Received: \n");	/* Setup to print the echoed string */

		memset(echoBuffer, '0', sizeof (echoBuffer));
		recv(sock, echoBuffer, sizeof (echoBuffer), 0);


		char *red = strtok(echoBuffer, " ");
		x = atoi(red);
		red = strtok(NULL, " ");
		y = atoi(red);
		red = strtok(NULL, " ");
		kolj = atoi(red);
		printf("x=%d, y=%d, количество жизней = %d\n", x, y, kolj);	/* Print the echo buffer */
		if (x == 66) {
			printf("Вы умерли\n");
			break;
		}
		if (x == 77) {
			printf
			    ("Вы вышли за пределы допустимой области, игра закончена\n");
			break;
		}

		printf("\n");	/* Print a final linefeed */
		sleep(1);
	}
	close(sock);
	exit(0);
}
