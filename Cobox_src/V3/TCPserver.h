#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef TCPSERVER_H
#define TCPSERVER_H
 
// Contenu du .h
 
typedef struct mesure {
    int id;
    double temp, hum;
    char timestamp[50];
}mesure; 
#endif