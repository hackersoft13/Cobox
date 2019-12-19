#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>
#include "TCPserver.h"
#ifndef EXTENSION_H
#define EXTENSION_H
 
// Contenu du .h
 
int recup_current_id(char *server, char *user, char *password, char *database);
int decide(double temp, double consigne);
int apply_order(int id_to_order, char *server, char *user, char *password, char *database);
#endif