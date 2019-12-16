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
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <mysql/mysql.h>

#include <time.h>

#include "TCPserver.h"

#define PORT 4444

int main(){
	key_t key = ftok("shmfile",65);
	int shmid = shmget(key,1024,0666|IPC_CREAT); 
	int sockfd, ret, i;
	struct sockaddr_in serverAddr;
	int newSocket;
	struct sockaddr_in newAddr;
	socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;
	mesure *m=NULL;
	m = (mesure*)malloc(sizeof(mesure)); 
	i=1;
	
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char *server = "localhost";
	char *user = "coco";
	char *password = "coco_pass"; /* set me first */
	char *database = "cobox_data";
	
	
	
	

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
						
						char tst[10];
						strcpy(tst,buffer);
						//printf("Copy OK\n");
						

						//printf("Device ID : %d\nTemperature : %d\nHumidity : %d\n", m[i].id, m[i].temp, m[i].hum);
						send(newSocket, "1;1;1\r", 10, 0);
						char *str = (char*) shmat(shmid,(void*)0,0); 
						//printf("Attach OK OK\n");
						strcpy(str,tst);
						printf("Ecriture du process %d dans la mémoire\n",getpid());
						shmdt(str);
						bzero(buffer, sizeof(buffer));
						
						
					}
				}
				i++;
			}
			//
			//exit(0);
		} else 
		{
			wait(NULL);
			int id_device,timestamp;
			double hum,temp;
			char *str = (char*) shmat(shmid,(void*)0,0);
			char req[255];
			id_device=atoi(strtok(str,";"));
			temp=atof(strtok(NULL,";"));
			hum=atof(strtok(NULL,";"));
			timestamp=(int)time(NULL);
			printf("ID : %d || Temp : %f || Hum : %f\n",id_device,temp,hum); 
			shmdt(str); 
			conn = mysql_init(NULL);
			printf("Ouverture de la base de données...\n");
			if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}else{
				printf("Ouverture OK\n");
			}
			//req=strcat("INSERT INTO mesure VALUES ('",i);
			//-----------Préparation de la requete SQL....

			sprintf(req,"INSERT INTO mesures (id, id_device, temp, hum, timestamp) VALUES(%d,%d,%f,%f,%d)",i, id_device, temp, hum, timestamp);

			if (mysql_query(conn, req)) {
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}else
			{
				printf("Insertion OK\n");
			}
			
			res = mysql_use_result(conn);
			mysql_free_result(res);
			printf("Fermeture BDD.\n");
			mysql_close(conn);
			

		}
		
		i++;

		

	}
	
	close(newSocket);
	shmctl(shmid,IPC_RMID,NULL); 


	return 0;
}