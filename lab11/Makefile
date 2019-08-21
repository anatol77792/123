all: client server 
client:
	gcc TCPEchoClient.c DieWithError.c -o client

server:
	gcc TCPEchoServer-Thread.c DieWithError.c AcceptTCPConnection.c CreateTCPServerSocket.c -lpthread -o server
	
clean:
	rm -f client server  
