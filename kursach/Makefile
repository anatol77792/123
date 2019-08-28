all: client server client2
client:
	gcc TCPEchoClient.c amessage.pb-c.c DieWithError.c -lprotobuf-c -o client
client2:
	gcc TCPEchoClient2.c amessage.pb-c.c DieWithError.c -lprotobuf-c -o client2
server:
	gcc TCPEchoServer-Thread.c amessage.pb-c.c DieWithError.c HandleTCPClient.c BroadcastSender.c BroadcastSender2.c AcceptTCPConnection.c CreateTCPServerSocket.c -lpthread -lprotobuf-c -o server

clean:
	rm -f client client2 server
