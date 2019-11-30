all: Cobox_Server Cobox_Client

Cobox_Server: TCP_server.o
	gcc -Wall TCP_server.o -o Cobox_Server

Cobox_Client: TCP_client.o
	gcc -Wall TCP_client.o -o Cobox_Client

TCP_server.o:
	gcc -c TCP_server.c

TCP_client.o:
	gcc -c TCP_client.c


clean:
	rm *.o Cobox_Server Cobox_Client 