#include "TCPEchoServer.h"	/* TCP echo server includes */
#include <pthread.h>		/* for POSIX threads */
#include <stdio.h>		/* for printf() and fprintf() */
#include <sys/socket.h>		/* for recv() and send() */
#include <unistd.h>		/* for close() */
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define NN 10
#define M 10
#define RCVBUFSIZE 1024
typedef struct DATA {
	int x;
	int y;
	int kolj;
	int clntSock;
} Data;
Data data[100];




void *ThreadMain(void *arg);	/* Main program of a thread */

/* Structure of arguments to pass to client thread */
struct ThreadArgs {
	int clntSock;		/* Socket descriptor for client */
	int A[NN][M];
	int h;
};

int main(int argc, char *argv[])
{
	int servSock;		/* Socket descriptor for server */
	int clntSock;		/* Socket descriptor for client */
	unsigned short echoServPort;	/* Server port */
	pthread_t threadID;	/* Thread ID from pthread_create() */
	struct ThreadArgs *threadArgs;	/* Pointer to argument structure for thread */
	int ii, j, i;
	if (argc != 2) {	/* Test for correct number of arguments */
		fprintf(stderr, "Usage:  %s <SERVER PORT>\n", argv[0]);
		exit(1);
	}


	echoServPort = atoi(argv[1]);	/* First arg:  local port */

	servSock = CreateTCPServerSocket(echoServPort);

	for (;;) {		/* run forever */
		clntSock = AcceptTCPConnection(servSock);

		/* Create separate memory for client argument */
		if ((threadArgs =
		     (struct ThreadArgs *) malloc(sizeof (struct ThreadArgs)))
		    == NULL)
			DieWithError("malloc() failed");
		threadArgs->clntSock = clntSock;
		srand(getpid());
		for (ii = 0; ii < NN; ii++) {
			for (j = 0; j < M; j++) {
				threadArgs->A[ii][j] = rand() % 3 - 1;
				printf("%d ", threadArgs->A[ii][j]);
				fflush(stdout);
			}
			puts("\n");
		}

		/* Create client thread */
		if (pthread_create
		    (&threadID, NULL, ThreadMain, (void *) threadArgs) != 0)
			DieWithError("pthread_create() failed");
		printf("with thread %ld\n", (long int) threadID);
	}
	/* NOT REACHED */
}

void *ThreadMain(void *threadArgs)
{
	int ii = 0;
	int j = 0;
	int clntSock;		/* Socket descriptor for client connection */
	int A[NN][M];
	/* Guarantees that thread resources are deallocated upon return */
	pthread_detach(pthread_self());

	/* Extract socket file descriptor from argument */
	clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;
	printf("jk=%d\n", clntSock);
	for (ii = 0; ii < NN; ii++) {
		for (j = 0; j < M; j++) {
			A[ii][j] = ((struct ThreadArgs *) threadArgs)->A[ii][j];
		}
	}
	free(threadArgs);	/* Deallocate memory for argument */
	data[clntSock].clntSock = clntSock;
	data[clntSock].kolj = 5;
	//  data[15].x= 9;
	//data[15].y= 4;
//      data[15].clntSock =15;
//      data[15].kolj = 15;
	const int BUFF_SIZE = 1024;
	char buff11[BUFF_SIZE];
	char echoBuffer[RCVBUFSIZE];	/* Buffer for echo string */
	int recvMsgSize;	/* Size of received message */
	/* Receive message from client */


	for (ii = 0; ii < NN; ii++) {
		for (j = 0; j < M; j++) {
			printf("%d ", A[ii][j]);
			fflush(stdout);
		}
		puts("\n");
	}
	for (int i = 0; i < 15; i++) {
		if ((recvMsgSize =
		     recv(clntSock, echoBuffer, RCVBUFSIZE, 0)) < 0) {
			DieWithError("recv() failed");
		}
		printf("%s\n", echoBuffer);
		char *red = strtok(echoBuffer, " ");
		data[clntSock].x = atoi(red);
		red = strtok(NULL, " ");
		data[clntSock].y = atoi(red);
		srand(getpid());
		if (data[clntSock].x < 0 || data[clntSock].y < 0
		    || data[clntSock].x > 9 || data[clntSock].y > 9) {
			sprintf(echoBuffer, "%d", 77);
			strcat(echoBuffer, " ");
			sprintf(buff11, "%d", data[clntSock].y);
			strcat(echoBuffer, buff11);
			strcat(echoBuffer, " ");
			sprintf(buff11, "%d", data[clntSock].kolj);
			strcat(echoBuffer, buff11);


			send(clntSock, echoBuffer, recvMsgSize, 0);
			break;
		}
		if (data[clntSock].kolj == 0) {
			data[clntSock].x = -1;
			data[clntSock].y = -1;
			sprintf(echoBuffer, "%d", 66);
			strcat(echoBuffer, " ");
			sprintf(buff11, "%d", data[clntSock].y);
			strcat(echoBuffer, buff11);
			strcat(echoBuffer, " ");
			sprintf(buff11, "%d", data[clntSock].kolj);
			strcat(echoBuffer, buff11);


			send(clntSock, echoBuffer, recvMsgSize, 0);
			break;
		}

		printf("x=%d, y=%d\n", data[clntSock].x, data[clntSock].y);
		fflush(stdout);

		data[clntSock].kolj =
		    data[clntSock].kolj + A[data[clntSock].x][data[clntSock].y];
		A[data[clntSock].x][data[clntSock].y] = 0;
		if (data[clntSock].kolj == 0) {
			data[clntSock].x = -1;
			data[clntSock].y = -1;
			sprintf(echoBuffer, "%d", 66);
			strcat(echoBuffer, " ");
			sprintf(buff11, "%d", data[clntSock].y);
			strcat(echoBuffer, buff11);
			strcat(echoBuffer, " ");
			sprintf(buff11, "%d", data[clntSock].kolj);
			strcat(echoBuffer, buff11);


			send(clntSock, echoBuffer, recvMsgSize, 0);
			break;
		}


		for (j = 0; j < 100; j++) {
			if (data[j].clntSock != clntSock) {
				if ((data[clntSock].x == data[j].x)
				    && (data[clntSock].y == data[j].y)) {
					if (data[j].kolj >= data[clntSock].kolj) {
						data[clntSock].kolj = 0;
						data[clntSock].x = -1;
						data[clntSock].y = -1;
						sprintf(echoBuffer, "%d", 66);
						strcat(echoBuffer, " ");
						sprintf(buff11, "%d",
							data[clntSock].y);
						strcat(echoBuffer, buff11);
						strcat(echoBuffer, " ");
						sprintf(buff11, "%d",
							data[clntSock].kolj);
						strcat(echoBuffer, buff11);

						if (send
						    (clntSock, echoBuffer,
						     recvMsgSize,
						     0) != recvMsgSize) {
							DieWithError
							    ("send() failed");
						}
						break;
					}

				}
			}
		}

		if (data[clntSock].kolj == 0) {
			break;
		}
		sprintf(echoBuffer, "%d", data[clntSock].x);
		strcat(echoBuffer, " ");

		sprintf(buff11, "%d", data[clntSock].y);
		strcat(echoBuffer, buff11);
		strcat(echoBuffer, " ");
		sprintf(buff11, "%d", data[clntSock].kolj);
		strcat(echoBuffer, buff11);

		if (send(clntSock, echoBuffer, recvMsgSize, 0) != recvMsgSize)
			DieWithError("send() failed");

	}

	close(clntSock);	/* Close client socket */

	return (NULL);
}
