#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

// Port d'écoute de la socket
#define PORT 8080
// Adresse d'écoute (toutes les adresses)
#define IP INADDR_ANY
// Taille de la file d'attente
#define BACKLOG 3
// Nombre de connexions clients
#define NB_CLIENTS 2
// Taille du tampon de lecture des messages
#define BUFFER_LEN 200
// Commande pour arrêter le serveur
#define EXIT_WORD "exit"

void initAdresse(struct sockaddr_in * adresse);
int initSocket(struct sockaddr_in * adresse);
int waitForClient(int * serverSocket);
void addClientToTab(int clientSocket, int clients[]);
void manageClient(int clients[]);

int main(void) {
	// Création et initialisation du tableau contenant les descripteurs des sockets clients
	int clients[NB_CLIENTS];
	for (int i = 0; i < NB_CLIENTS; i++) {
		clients[i] = -1;
	}
	// Structure contenant l'adresse
	struct sockaddr_in adresse;
	initAdresse(&adresse);
	// Descripteur de la socket du serveur
	int serverSocket = initSocket(&adresse);
	int clientSocket;
	while (1) {
		// Descripteur de la socket du client, on attend une connexion
		if ((clientSocket = waitForClient(&serverSocket)) != -1) {
			// On ajoute le nouveau client au tableau des descripteurs
			addClientToTab(clientSocket, clients);
		}
		manageClient(clients);
	}
	return EXIT_SUCCESS;
}
// Initialisation de la structure sockaddr_in
void initAdresse(struct sockaddr_in * adresse) {
	(*adresse).sin_family = AF_INET;
	(*adresse).sin_addr.s_addr = IP;
	(*adresse).sin_port = htons( PORT);
}
// Démarrage de la socket serveur
int initSocket(struct sockaddr_in * adresse) {
	// Descripteur de socket
	int fdsocket;
	// Nombre d'options
	int opt = 1;
	// Création de la socket en TCP
	if ((fdsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Echéc de la création: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("Création de la socket\n");
	// Paramètrage de la socket
	if (setsockopt(fdsocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
			sizeof(opt)) != 0) {
		printf("Echéc de paramètrage: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("Paramètrage de la socket\n");
	// Attachement de la socket sur le port et l'adresse IP
	if (bind(fdsocket, (struct sockaddr *) adresse, sizeof(*adresse)) != 0) {
		printf("Echéc d'attachement: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("Attachement de la socket sur le port %i\n", PORT);
	// Passage en écoute de la socket
	if (listen(fdsocket, BACKLOG) != 0) {
		printf("Echéc de la mise en écoute: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	// Passage en mode non bloquant, on va écouter ce qu'il se passe
	fcntl(fdsocket, F_SETFL, O_NONBLOCK);
	printf("Mise en écoute de la socket\n");
	return fdsocket;
}
// Attente de connexion d'un client
int waitForClient(int * serverSocket) {
	int clientSocket;
	// Structure contenant l'adresse du client
	struct sockaddr_in clientAdresse;
	int addrLen = sizeof(clientAdresse);
	if ((clientSocket = accept(*serverSocket, (struct sockaddr*) &clientAdresse,
			(socklen_t*) &addrLen)) != -1) {
		// Convertion de l'IP en texte
		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(clientAdresse.sin_addr), ip, INET_ADDRSTRLEN);
		printf("Connexion de %s:%i\n", ip, clientAdresse.sin_port);
		// Passage en mode non bloquant
		int opt = 1;
		setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(1));
	}
	return clientSocket;
}
// Ajoute les clients au tableau
void addClientToTab(int clientSocket, int clients[]) {
	// On vérifie si on à de la place de libre
	int found = -1;
	for (int i = 0; i < NB_CLIENTS; i++) {
		// On cherche de la place
		if (clients[i] == -1) {
			// On ajoute le client
			printf("Ajout du client à l'index %i\n", i);
			clients[i] = clientSocket;
			// Nouvelle connexion, envoie du message de bienvenu
			send(clientSocket, "Entrez 'exit' pour quitter\n", 	strlen("Entrez 'exit' pour quitter\n"),
					MSG_DONTWAIT);
			found = 0;
			break;
		}
	}
	if (found == -1) {
		// On a plus de place de libre
		puts("Plus de place, désolé");
		close(clientSocket);
	}
}
// On traite l'input des clients
void manageClient(int clients[]) {
	// Création d'un tampon pour stocker les messages des clients dans la heap
	static char buffer[BUFFER_LEN + 1];
	for (int i = 0; i < NB_CLIENTS; i++) {
		// On vérifie les messages
		int clientSocket = clients[i];
		if (clientSocket == -1) {
			continue;
		}
		int len = recv(clientSocket, buffer, BUFFER_LEN, MSG_DONTWAIT);
		// Booléen pour suivre l'état de la socket
		int isClosed = 0;
		if (len == -1 && errno != EAGAIN) {
			// Une erreur est survenue
			printf("Errno [%i] : %s\n", errno, strerror(errno));
			isClosed = 1;
		} else if (len == 0) {
			// Le client s'est déconnecté (extrémité de la socket fermée)
			isClosed = 1;
		} else if (len > 0) {
			// Ajout du terminateur de chaîne
			buffer[len] = '\0';
			if (strncmp(buffer, EXIT_WORD, 4) == 0) {
				// Le client veut se déconnecter
				send(clientSocket, "Bye\n", strlen("Bye\n"), 0);
				isClosed = 1;
			} else {
				// On renvoie le texte au client dans un buffer assez grand
				int len = strlen("Vous avez dit : ") + strlen(buffer) + 1;
				char response[len];
				strcpy(response, "Vous avez dit : ");
				strcat(response, buffer);
				// Un seul envoie permet de ne pas surcharger le réseau
				send(clientSocket, response, strlen(response), 0);
			}
		}
		if (isClosed == 1) {
			// La socket est fermé ou le client veut quitter le serveur !
			printf("Fermeture de la connexion avec le client\n");
			// Fermeture de la socket
			close(clientSocket);
			// On fait de la place dans le tableau
			clients[i] = -1;
		}
	}
}