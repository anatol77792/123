#include <stdio.h>		/* for printf() and fprintf() */
#include <sys/socket.h>		/* for recv() and send() */
#include <unistd.h>		/* for close() */
#include "TCPEchoServer.h"	/* TCP echo server includes */

#define MAX_MSG_SIZE 1024
#define FLA 40
#define RCVBUFSIZE 255		/* Size of receive buffer */
#include "amessage.pb-c.h"

void DieWithError(char *errorMessage);	/* Error handling function */


void HandleTCPClient(int clntSocket, unsigned short echo, int clntSocket2)
{

	if (echo == 10) {
		msgqid = msgget(MSGKEY, MSGPERM | IPC_EXCL);
		if (msgqid < 0) {
			perror(strerror(errno));
			printf
			    ("failed to create message queue with msgqid = %d\n",
			     msgqid);
		}
		rc = msgrcv(msgqid, &msg1, sizeof (msg1) - sizeof (long),
			    1, 0);
		if (rc < 0) {
			perror(strerror(errno));
			printf("msgrcv failed, rc=%d\n", rc);
		}


		AMessage msg = AMESSAGE__INIT;	// AMessage
		void *buf;	// Buffer to store serialized data
		unsigned lenn;	// Length of serialized data


		msg.t = msg1.t;
		msg.len = msg1.lens;
		msg.strr = msg1.mtext;
		lenn = amessage__get_packed_size(&msg);
		buf = malloc(lenn);
		amessage__pack(&msg, buf);
		if (send(clntSocket2, buf, lenn, 0) != lenn) {

			DieWithError
			    ("send() sent a different number of bytes than expected");
		}
		printf("Отправлено %d %d %s\n", msg.t, msg.len,
		       msg.strr);
		close(clntSocket2);



		free(buf);

		och = och - 1;

		for (int i = 2; i <= och; i++) {
			rc = msgrcv(msgqid, &msg1,
				    sizeof (msg1) - sizeof (long), i, 0);
			if (rc < 0) {
				perror(strerror(errno));
				printf("msgrcv failed, rc=%d\n", rc);
			}

			msg1.mtype = i - 1;
			msg1.len = sizeof (msg1) - sizeof (long);	//getpid();
			rc = msgsnd(msgqid, &msg1,
				    sizeof (msg1) - sizeof (long), 0);
			if (rc < 0) {
				perror(strerror(errno));
				printf("msgsnd failed, rc = %d\n", rc);
			}

		}

		gt = 5;

	}
	if (echo == 0) {
		AMessage *msgg;
		uint8_t buf[MAX_MSG_SIZE];
		size_t cur_len = 0;
		size_t nread = 0;

		while ((nread =
			recv(clntSocket, buf + cur_len,
			     MAX_MSG_SIZE - cur_len, 0)) != 0) {
			cur_len += nread;
			if (cur_len == MAX_MSG_SIZE) {
				fprintf(stderr,
					"max message length exceeded\n");
				exit(1);
			}
		}
		size_t msg_len = cur_len;
		msgg = amessage__unpack(NULL, msg_len, buf);
		if (msgg == NULL) {
			fprintf(stderr,
				"error unpacking incoming message\n");
			exit(1);
		}
		sprintf(data1.str, "%s", msgg->strr);
		data1.len = msgg->len;
		data1.t = msgg->t;
		amessage__free_unpacked(msgg, NULL);
		msg1.mtype = och;	// set the type of message
		och = och + 1;
		if (och > 5) {
			gt = 0;
		}
		msg1.len = sizeof (msg1) - sizeof (long);	//getpid();
		msg1.lens = data1.len;
		msg1.t = data1.t;
		sprintf(msg1.mtext, "%s\n", data1.str);
		printf("Получено: %d %d %s\n", msg1.t, msg1.lens,
		       msg1.mtext);
		rc = msgsnd(msgqid, &msg1, sizeof (msg1) - sizeof (long),
			    0);
		if (rc < 0) {
			perror(strerror(errno));
			printf("msgsnd failed, rc = %d\n", rc);
		}

		close(clntSocket);	/* Close client socket */
	}


}
