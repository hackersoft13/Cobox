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