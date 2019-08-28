#include "TCPEchoServer.h"	/* TCP echo server includes */
#include <pthread.h>		/* for POSIX threads */
void *ThreadMain(void *arg);	/* Main program of a thread */
int servSock;			/* Socket descriptor for server */
int servSock2;
int clntSock2;
int clntSock;
/* Structure of arguments to pass to client thread */
struct ThreadArgs {
	int clntSock;		/* Socket descriptor for client */
	int clntSock2;
	unsigned short echo;
};
struct ThreadArgs *threadArgs;
typedef struct DATA {
	char *broadcastIP;
	unsigned short broadcastPort;
	unsigned short broadcastPort2;
	unsigned short echoServPort;
	unsigned short echoServPort2;
} Data;
Data data;
void *thread_func(void *arg)
{

	//pthread_detach(pthread_self()); 
	Data *a = (Data *) arg;
	BroadcastSender(a->broadcastIP, a->broadcastPort);

	return (NULL);
}

void *thread_func3(void *arg)
{

	//pthread_detach(pthread_self()); 
	Data *aaa = (Data *) arg;
	BroadcastSender2(aaa->broadcastIP, aaa->broadcastPort2);

	return (NULL);
}

void *thread_func2(void *arg)
{
	/* Thread ID from pthread_create() */
	//pthread_detach(pthread_self());
	pthread_t threadID;

	Data *aa = (Data *) arg;
	servSock = CreateTCPServerSocket(aa->echoServPort);
	for (;;) {		/* run forever */
		clntSock = AcceptTCPConnection(servSock);
		pthread_mutex_init(&mutex, NULL);
		/* Create separate memory for client argument */
		if ((threadArgs =
		     (struct ThreadArgs *)
		     malloc(sizeof (struct ThreadArgs)))
		    == NULL)
			DieWithError("malloc() failed");
		threadArgs->clntSock = clntSock;
		threadArgs->echo = 0;
		/* Create client thread */

		if (pthread_create
		    (&threadID, NULL, ThreadMain,
		     (void *) threadArgs) != 0)
			DieWithError("pthread_create() failed");
		printf("with thread %ld\n", (long int) pthread_self());
		pthread_mutex_destroy(&mutex);
	}

	return (NULL);
}
void *thread_func4(void *arg)
{
	/* Thread ID from pthread_create() */
	//pthread_detach(pthread_self());
	pthread_t threadID;

	Data *aa = (Data *) arg;
	servSock2 = CreateTCPServerSocket(aa->echoServPort2);
	for (;;) {		/* run forever */
		clntSock2 = AcceptTCPConnection(servSock2);
	pthread_mutex_init(&mutex, NULL);
		/* Create separate memory for client argument */
		if ((threadArgs =
		     (struct ThreadArgs *)
		     malloc(sizeof (struct ThreadArgs)))
		    == NULL)
			DieWithError("malloc() failed");
		threadArgs->clntSock2 = clntSock2;
		threadArgs->echo = 10;
		/* Create client thread */

		if (pthread_create
		    (&threadID, NULL, ThreadMain,
		     (void *) threadArgs) != 0)
			DieWithError("pthread_create() failed");
		printf("with thread %ld\n", (long int) pthread_self());
		pthread_mutex_destroy(&mutex);
	}

	return (NULL);
}

int main(int argc, char *argv[])
{
	/* Socket descriptor for client */
	unsigned short echoServPort;	/* Server port */
	unsigned short echoServPort2;	/* Server port */
	unsigned short broadcastPort;
	unsigned short broadcastPort2;
	char *broadcastIP;
	msgqid = msgget(MSGKEY, MSGPERM | IPC_EXCL);
	msgctl(msgqid, IPC_RMID, NULL);
	pthread_t thread1, thread2, thread3, thread4;


	if (argc != 3) {	/* Test for correct number of arguments */
		fprintf(stderr, "Usage:  %s <SERVER PORT1><SERVER PORT1>\n", argv[0]);
		exit(1);
	}
	gt = 5;
	och = 1;
	echoServPort = atoi(argv[1]);	/* First arg:  local port */
	echoServPort2 = atoi(argv[2]);	/* First arg:  local port */
	broadcastIP = "127.255.255.255";
	broadcastPort = echoServPort + 30;
	data.broadcastPort = broadcastPort;
	data.broadcastIP = broadcastIP;
	data.echoServPort = echoServPort;
	data.echoServPort2 = echoServPort2;
	broadcastPort2 = echoServPort + 1000;
	data.broadcastPort2 = broadcastPort2;
	

	msgqid = msgget(MSGKEY, MSGPERM | IPC_CREAT | IPC_EXCL);
	if (msgqid < 0) {
		perror(strerror(errno));
		printf("failed to create message queue with msgqid = %d\n",
		       msgqid);
		return 1;
	} else
		printf("message queue %d created\n", msgqid);

	pthread_create(&thread1, NULL, thread_func, &data);
	pthread_create(&thread2, NULL, thread_func2, &data);
	pthread_create(&thread3, NULL, thread_func3, &data);
	pthread_create(&thread4, NULL, thread_func4, &data);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
}

void *ThreadMain(void *threadArgs)
{
	int clntSock;		/* Socket descriptor for client connection */
    int echo;
	/* Guarantees that thread resources are deallocated upon return */
	pthread_detach(pthread_self());

	/* Extract socket file descriptor from argument */
	clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;
	clntSock2 = ((struct ThreadArgs *) threadArgs)->clntSock2;
	echo = ((struct ThreadArgs *) threadArgs)->echo;
	free(threadArgs);	/* Deallocate memory for argument */
	pthread_mutex_lock(&mutex);
	HandleTCPClient(clntSock, echo, clntSock2);
	pthread_mutex_unlock(&mutex);
	return (NULL);
}
