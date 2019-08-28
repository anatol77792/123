#include <stdio.h>		/* for printf() and fprintf() */
#include <sys/socket.h>		/* for recv() and send() */
#include <unistd.h>		/* for close() */
#include "TCPEchoServer.h"	/* TCP echo server includes */
#include "amessage.pb-c.h"
#define MAX_MSG_SIZE 1024
#define FLA 40
#define RCVBUFSIZE 255		/* Size of receive buffer */

void DieWithError(char *errorMessage);	/* Error handling function */


void HandleTCPClient(int clntSocket)
{

	AMessage *msgg;
	uint8_t buf[MAX_MSG_SIZE];
	size_t cur_len = 0;
	size_t nread=0;
	/*while ((nread=fread(buf + cur_len, 1, MAX_MSG_SIZE - cur_len, clntSocket)) != 0)
	   {
	   cur_len += nread;
	   if (cur_len == MAX_MSG_SIZE)
	   {
	   fprintf(stderr, "max message length exceeded\n");
	   exit(1);
	   }
	   } */

	while ((nread =
		recv(clntSocket, buf + cur_len, MAX_MSG_SIZE - cur_len,
		     0)) != 0) {
		cur_len += nread;
			if (cur_len == MAX_MSG_SIZE)
    {
      fprintf(stderr, "max message length exceeded\n");
      exit(1);
    }
	}
	printf("Получено1: \n");
	/*if ((recvMsgSize =
	   recv(clntSocket, buf+ cur_len, MAX_MSG_SIZE- cur_len, 0)) < 0) {
	   DieWithError("recv() failed");
	   } */
	size_t msg_len = cur_len;
	msgg = amessage__unpack(NULL, msg_len, buf);
	if (msgg == NULL) {
		fprintf(stderr, "error unpacking incoming message\n");
		exit(1);
	}
	sprintf(data1.str, "%s", msgg->strr);
	data1.len = msgg->len;
	printf("received msg: %s\n", msgg->strr);
	data1.t = msgg->t;
	amessage__free_unpacked(msgg, NULL);
	// printf("received 1oiuy: %s\n", data1.str);
	if (data1.t != FLA) {
		msg1.mtype = och;	// set the type of message
		och = och + 1;
		if (och > 5) {
			gt = 0;
		}
		msg1.len = sizeof (msg1) - sizeof (long);	//getpid();
		msg1.lens = data1.len;
		msg1.t = data1.t;
		sprintf(msg1.mtext, "%s\n", data1.str);
		printf("msg: %d %d %s\n", msg1.t, msg1.lens, msg1.mtext);
		rc = msgsnd(msgqid, &msg1, sizeof (msg1) - sizeof (long), 0);
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
		CMessage msggg = CMESSAGE__INIT;	// AMessage
		void *buff;	// Buffer to store serialized data
		unsigned lenn;	// Length of serialized data
		msgqid = msgget(MSGKEY, MSGPERM | IPC_EXCL);
		if (msgqid < 0) {
			perror(strerror(errno));
			printf
			    ("failed to create message queue with msgqid = %d\n",
			     msgqid);
		}
		rc = msgrcv(msgqid, &msg1, sizeof (msg1) - sizeof (long), 1,
			    0);
		if (rc < 0) {
			perror(strerror(errno));
			printf("msgrcv failed, rc=%d\n", rc);
		}
		printf("received msg: %d %d %s\n", msg1.t, msg1.lens,
		       msg1.mtext);
		data1.len = msg1.lens;
		sprintf(data1.str, "%s", msg1.mtext);
		data1.t = msg1.t;
		msggg.t = msg1.t;
		msggg.len = msg1.len;
		msggg.strr = msg1.mtext;
		lenn = cmessage__get_packed_size(&msggg);
		buff = malloc(lenn);
		cmessage__pack(&msggg, buff);
		
if (send(clntSocket, buff, lenn, 0) !=
		    lenn)
			DieWithError
			    ("send() sent a different number of bytes than expected");

		//send(clntSocket, buff, lenn, 0);
		//send(clntSocket, buf, lenn, 0);
		och = och - 1;

		for (int i = 2; i <= och; i++) {
			rc = msgrcv(msgqid, &msg1,
				    sizeof (msg1) - sizeof (long), i, 0);
			if (rc < 0) {
				perror(strerror(errno));
				printf("msgrcv failed, rc=%d\n", rc);
			}
//      sprintf (msg.mtext, "%s\n", echoBuffer); 
			//printf("prov: %s\n", msg.mtext);
			msg1.mtype = i - 1;
			msg1.len = sizeof (msg1) - sizeof (long);	//getpid();
			rc = msgsnd(msgqid, &msg1,
				    sizeof (msg1) - sizeof (long), 0);
			if (rc < 0) {
				perror(strerror(errno));
				printf("msgsnd failed, rc = %d\n", rc);
			}

		}
free(buff);
		gt = 5;
	}

	close(clntSocket);	/* Close client socket */
}
