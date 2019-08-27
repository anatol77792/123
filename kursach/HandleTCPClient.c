#include <stdio.h>		/* for printf() and fprintf() */
#include <sys/socket.h>		/* for recv() and send() */
#include <unistd.h>		/* for close() */
#include "TCPEchoServer.h"	/* TCP echo server includes */
#define FLA 40
#define RCVBUFSIZE 32		/* Size of receive buffer */

void DieWithError(char *errorMessage);	/* Error handling function */

void HandleTCPClient(int clntSocket)
{
	char echoBuffer[RCVBUFSIZE];	/* Buffer for echo string */
	int recvMsgSize;	/* Size of received message */
	/* Receive message from client */
	if ((recvMsgSize =
	     recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0) {
		DieWithError("recv() failed");
	}

	if (atoi(echoBuffer) != FLA) {
		msg.mtype = och;	// set the type of message
		och = och + 1;
		if (och > 5) {
			gt = 0;
		}
		msg.len = sizeof (msg) - sizeof (long);	//getpid();

		sprintf(msg.mtext, "%s\n", echoBuffer);
		printf("received 1: %s\n", msg.mtext);
		rc = msgsnd(msgqid, &msg, sizeof (msg) - sizeof (long), 0);
		if (rc < 0) {
			perror(strerror(errno));
			printf("msgsnd failed, rc = %d\n", rc);
		}

		/*
		   while (recvMsgSize > 0)      
		   {

		   if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
		   DieWithError("send() failed");

		   if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
		   DieWithError("recv() failed");
		   } */
	} else {
		msgqid = msgget(MSGKEY, MSGPERM | IPC_EXCL);
		if (msgqid < 0) {
			perror(strerror(errno));
			printf
			    ("failed to create message queue with msgqid = %d\n",
			     msgqid);
		}
		rc = msgrcv(msgqid, &msg, sizeof (msg) - sizeof (long), 1,
			    0);
		if (rc < 0) {
			perror(strerror(errno));
			printf("msgrcv failed, rc=%d\n", rc);
		}
		printf("received msg: %s\n", msg.mtext);
		send(clntSocket, msg.mtext, recvMsgSize, 0);
		och = och - 1;

		for (int i = 2; i <= och; i++) {
			rc = msgrcv(msgqid, &msg,
				    sizeof (msg) - sizeof (long), i, 0);
			if (rc < 0) {
				perror(strerror(errno));
				printf("msgrcv failed, rc=%d\n", rc);
			}
//      sprintf (msg.mtext, "%s\n", echoBuffer); 
			//printf("prov: %s\n", msg.mtext);
			msg.mtype = i - 1;
			msg.len = sizeof (msg) - sizeof (long);	//getpid();
			rc = msgsnd(msgqid, &msg,
				    sizeof (msg) - sizeof (long), 0);
			if (rc < 0) {
				perror(strerror(errno));
				printf("msgsnd failed, rc = %d\n", rc);
			}

		}

		gt = 5;
	}

	close(clntSocket);	/* Close client socket */
}
