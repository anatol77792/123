#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>

#define MAX_SEND_SIZE 80
int hran = 50;
void sigquit()
{
	printf("My DADDY has Killed me!!!\n");
	exit(0);
}

struct mymsgbuf {
	long mtype;
	char mtext[MAX_SEND_SIZE];
};

int msgqid, rc;

void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text)
{
	qbuf->mtype = type;
	strcpy(qbuf->mtext, text);
	if ((msgsnd(qid, (struct msgbuf *) qbuf, strlen(qbuf->mtext) + 1, 0)) ==
	    -1) {
		perror("msgsnd");
		exit(1);
	}
}

int read_message(int qid, struct mymsgbuf *qbuf, long type, int jii)
{
	qbuf->mtype = type;
	msgrcv(qid, (struct msgbuf *) qbuf, MAX_SEND_SIZE, type, 0);
	jii = jii + atoi(qbuf->mtext);
	printf("Мёда в хранилище: %d\n", jii);

	return jii;
}

int main(int argc, char *argv[])
{

	key_t key;
	int qtype = 1;
	struct mymsgbuf qbuf;

	if (argc < 2) {
		printf("Usage: количество пчел\n");
		exit(-1);
	}
	int count = atoi(argv[1]);
	int i, pid[count];
	key = ftok(".", 'm');
	if ((msgqid = msgget(key, IPC_CREAT | 0660)) == -1) {
		perror("msgget");
		exit(1);
	}

	for (i = 1; i <= count; i++) {
		// запускаем дочерний процесс 
		pid[i] = fork();
		srand(getpid());

		if (-1 == pid[i]) {
			perror("fork");	/* произошла ошибка */
			exit(1);	/*выход из родительского процесса */
		} else if (0 == pid[i]) {
			if (i != count) {
				printf
				    (" CHILD: Это  пчела СТАРТ!\n");


				while (1) {
					char str[10];
					int med = 5;


					sleep(rand() % 6);
					sprintf(str, "%d", med);
					send_message(msgqid,
						     (struct mymsgbuf *) &qbuf,
						     qtype, str);
					printf
					    (" CHILD: Это %d пчела добыла мед!\n",
					     i);
					fflush(stdout);
					sleep(rand() % 6);
				}
			}
			if (i == count) {
				printf
				    (" CHILD: Это  пчела СТАРТ!\n");


				while (1) {
					char str[10];
					int med = -5;
					signal(SIGQUIT, sigquit);

					sleep(rand() % 6);
					sprintf(str, "%d", med);
					send_message(msgqid,
						     (struct mymsgbuf *) &qbuf,
						     qtype, str);
					printf
					    (" CHILD: Это медведь поел!\n");
					fflush(stdout);
					sleep(rand() % 2);
					signal(SIGKILL, sigquit);
				}
			}
			exit(0);	/* выход из процесс-потомока */
		}
	}

	int ji = 20;




	while (ji > 0 && ji < hran) {

		ji = read_message(msgqid, &qbuf, qtype, ji);

	}
	if (ji <= 0) {
		printf("Мишка умер с голодухи\n");
	}

	if (ji >= hran) {
		printf("Хранилище заполнено\n");
	}
	for (int i = 1; i <= count; i++) {
		kill(pid[i], SIGKILL);
	}
	if ((rc = msgctl(msgqid, IPC_RMID, NULL)) < 0) {
		perror(strerror(errno));
		printf("msgctl (return queue) failed, rc=%d\n", rc);
		return 1;
	}

	return 0;
}
