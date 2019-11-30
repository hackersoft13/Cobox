#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include<sys/wait.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include "TCPserver.h"
#define PORT 4444

int main(){
	int fd;
	mkfifo("fifo", 0666);
	int sockfd, ret, i;
	struct sockaddr_in serverAddr;
	int newSocket;
	struct sockaddr_in newAddr;
	socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;
	mesure *m=NULL;
	m = (mesure*)malloc(sizeof(mesure)); 
	i=0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		printf("Mesure n° %d\n", i);
		childpid = fork();
		if(childpid == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if (strlen(buffer) > 3){
					if(strcmp(buffer, ":exit") == 0){
						printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
						exit(0);
						break;
					}else{
						
						printf("Ouverture FIFO\n");

						fd=open("fifo", O_RDWR);
						write(fd, "COUCOU\n", (strlen("COUCOU\n")+1));
						(m+i)->id=atoi(strtok(buffer, ";"));
						(m+i)->temp=atoi(strtok(NULL,";"));
						(m+i)->hum=atoi(strtok(NULL,";"));
						m = realloc(m,(sizeof(m))+sizeof(mesure));
						printf("Device ID : %d\nTemperature : %d\nHumidity : %d\n", m[i].id, m[i].temp, m[i].hum);
						send(newSocket, "1;1;1\r", 10, 0);
						//sleep(4);
						bzero(buffer, sizeof(buffer));
						close (fd);
						i++;
					}
				}
				
			}
			//
			//exit(0);
		} else 
		{
			char tst[50];
			pid_t cpid = wait(NULL);
			fd = open("fifo", O_RDONLY | O_NONBLOCK);
			read(fd, tst, (strlen("COUCOU\n")+1));
			printf("%s",tst);
		}
		
		i++;

		

	}
	
	close(newSocket);


	return 0;
}