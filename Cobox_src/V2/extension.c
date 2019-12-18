#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>
#include "TCPserver.h"

int recup_current_id(char *server, char *user, char *password, char *database){
    MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
    char req[255];
    int id_recup;
    conn = mysql_init(NULL);
    //printf("Ouverture de la base de données...\n");
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}else{
		//printf("Ouverture OK\n");
	}
    sprintf(req,"SELECT MAX(id) FROM mesures;");

	if (mysql_query(conn, req)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}else
	{
    	//printf("Récupération ID OK\n");
	}

	res = mysql_use_result(conn);
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) 
    { 
    for(int i = 0; i < num_fields; i++) 
    { 
        //printf("%s ", row[i] ? row[i] : "NULL"); 
        id_recup = atoi(row[i]);
    } 
        //printf("\n"); 
    }
	mysql_free_result(res);
	//printf("Fermeture BDD.\n");
	mysql_close(conn);
    return id_recup;
}

int decide(float temp, float consigne){
	//Décide d'inscrire l'ordre à donner en fonctoin d'une consigne

}

int apply_order(int id_to_order, char *server, char *user, char *password, char *database){
	//Applique l'ordre (si il y en à un) à l'actionneur
	//Si l'actionneur n'a pas d'ordre en attente, on vérifie si il ne doit pas arreter un précédent ordre
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
    char req[255];
    int ordre, intern_device;
    conn = mysql_init(NULL);
    //printf("Ouverture de la base de données...\n");
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}else{
		//printf("Ouverture OK\n");
	}
    sprintf(req,"SELECT order FROM ordres WHERE id_device=%d;",id_to_order);

	if (mysql_query(conn, req)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}else
	{
    	//printf("Récupération ID OK\n");
	}

	res = mysql_use_result(conn);
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) 
    { 
    for(int i = 0; i < num_fields; i++) 
    { 
        //printf("%s ", row[i] ? row[i] : "NULL"); 
        ordre = atoi(row[i]);
    } 
        //printf("\n"); 
    }
	mysql_free_result(res);
	//printf("Fermeture BDD.\n");
	mysql_close(conn);
    return ordre;
}