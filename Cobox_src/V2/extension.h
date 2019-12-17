#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef EXTENSION_H
#define EXTENSION_H
 
// Contenu du .h
 
int recup_current_id(char *server, char *user, char *password, char *database);
#endif