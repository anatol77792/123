#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>

#define MSGPERM 0600    // msg queue permission
#define MSGTXTLEN 128   // msg text length
#define MSGKEY 32786
struct msg_buf {
  long mtype;
  char mtext[MSGTXTLEN];
  int len;
} msg;
int msgqid, rc;

void DieWithError(char *errorMessage);  /* Error handling function */
void HandleTCPClient(int clntSocket);   /* TCP client handling function */
void BroadcastSender(char *broadcastIP, unsigned short broadcastPort); 
void BroadcastSender2(char *broadcastIP, unsigned short broadcastPort); 
int CreateTCPServerSocket(unsigned short port); /* Create TCP server socket */
int AcceptTCPConnection(int servSock);  /* Accept TCP connection request */
int gt;
int och;
pthread_mutex_t mutex;
