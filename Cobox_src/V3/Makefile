all: Cobox_Server Cobox_Client

Cobox_Server: TCP_server.o extension.o
	gcc -Wall TCP_server.o extension.o -o Cobox_Server -L/usr/lib/arm-linux-gnueabihf  -lmariadbclient -lpthread -lz -lm -ldl

Cobox_Client: TCP_client.o
	gcc -Wall TCP_client.o -o Cobox_Client

TCP_server.o:
	gcc -c TCP_server.c -I/usr/include/mysql -L/usr/lib/arm-linux-gnueabihf  -lmariadbclient -lpthread -lz -lm -ldl

extension.o:
	gcc -c extension.c

TCP_client.o:
	gcc -c TCP_client.c


clean:
	rm *.o Cobox_Server Cobox_Client 
