all: client server client2
client:
	gcc TCPEchoClient.c DieWithError.c -o client
client2:
	gcc TCPEchoClient2.c DieWithError.c -o client2
server:
	gcc TCPEchoServer-Thread.c DieWithError.c HandleTCPClient.c BroadcastSender.c BroadcastSender2.c AcceptTCPConnection.c CreateTCPServerSocket.c -lpthread -o server

clean:
	rm -f client client2 server
