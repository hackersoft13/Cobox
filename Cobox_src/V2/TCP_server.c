#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <sys/stat.h>

#include "TCPserver.h"
#define PORT 4444

int main()
{
	// Création de la FIFO
	//char * myfifo = "/home/paul/Documents/Cobox/Cobox/Cobox_src/V2/fifo";
	//mkfifo(myfifo, 0766);
	// Fin création FIFO
	//char *fifosend = "/home/paul/Documents/Cobox/Cobox/Cobox_src/V2/fifo";
	int fd;
	mkfifo("fifo", 0666);
	//----------Début socket
	int sockfd, ret, i;
	struct sockaddr_in serverAddr;
	int newSocket;
	struct sockaddr_in newAddr;
	socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;
	//---------Fin Socket
	mesure *m = NULL;
	m = (mesure *)malloc(sizeof(mesure));
	i = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if (listen(sockfd, 10) == 0)
	{
		printf("[+]Listening....\n");
	}
	else
	{
		printf("[-]Error in binding.\n");
	}

	while (1)
	{
		newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
		if (newSocket < 0)
		{
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		printf("Mesure n° %d\n", i);
		childpid = fork();
		if (childpid == 0)
		{
			close(sockfd);

			while (1)
			{
				recv(newSocket, buffer, 1024, 0);
				if (strlen(buffer) > 3)
				{

					if (strcmp(buffer, ":exit") == 0)
					{
						printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
						exit(0);
						break;
					}
					else
					{
						printf("Ouverture FIFO\n");

						fd=open("fifo", O_RDWR);

						//close(fd);
						char str2[80];
						(m + i)->id = atoi(strtok(buffer, ";"));
						(m + i)->temp = atoi(strtok(NULL, ";"));
						(m + i)->hum = atoi(strtok(NULL, ";"));
						m = realloc(m, (sizeof(m)) + sizeof(mesure));
						printf("Device ID : %d\nTemperature : %d\nHumidity : %d\n", m[i].id, m[i].temp, m[i].hum);
						strcpy(str2, "1;23;54\n");
						write(fd, str2, sizeof(str2));
						close(fd);
						send(newSocket, "1;1;1\r", 10, 0);

						//sleep(4);
						bzero(buffer, sizeof(buffer));
						i++;
					}
				}
			}
			//
			//exit(0);
		}
		else
		{
			wait(NULL);
			char input;
			int n;
			//----Récupération de la données via FIFO
			//fd1 = open(myfifo,O_RDONLY);
			//read(fd1, str1, strlen(buffer));

			// Print the read string and close
			//printf("User1: %s\n", str1);
			//close(fd1);
			char str1[80];
			
			fd = open("fifo", O_RDONLY);
			printf("Ouverture de reception\n");
			while((n=read(fd, &input, 1)) > 0)
			{
				printf(" char : %c",input);
			}
			close(fd);
		}

		i++;
	}

	close(newSocket);

	return 0;
}