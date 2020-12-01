
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>

#include "funcmail.h"

#define REQUEST_MSG_SIZE 1024
#define REPLY_MSG_SIZE 5000
#define SERVER_PORT_NUM 25

FILE * fp;

char valores[1024];

static int callback(void *buffer, int argc, char **argv, char**azColName) {
	int i;
	
	for(i = 0; i < argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] :"NULL");
		strcpy(valores, argv[i]);
		
	}
	
	printf("\n");
	
	return 0;
}


int main(int argc, char *argv[])
{

	char nom_servidor[32] = "172.20.0.21"; //Adreça IP per accedir al server
	char email_destinatari[REPLY_MSG_SIZE] = "1458570@campus.euss.org"; //Email del destinatari
	char email_remitent[REPLY_MSG_SIZE] = "afontquerni@euss.es"; //Email del remitent
	char text_email[REPLY_MSG_SIZE]; //Text del email.
	char minim[1024];
	char maxim[1024];
	sqlite3 *db;
	char *zErrMsg = 0;
	char buffer[1024];
	char text[1024];
	char sql[1024];
	memset(buffer, '\0', 1024);
	int rc;
	
	rc = sqlite3_open("/home/pi/Desktop/FITA1/fita5web/temperatures.db", &db);
	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}

	fp = fopen("/home/pi/Desktop/FITA1/fita5web/informeTemperatures.txt", "w");
	if (fp == NULL) {
		printf("Can't open the file.\n");
	}

    //Data i hora inicial
    sprintf(sql, "SELECT MIN(Temps) FROM Lectures %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Data i hora inici lectura: %s\n", valores) ;

    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));
 
    //Data i hora final
    sprintf(sql, "SELECT MAX(Temps) FROM Lectures %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Data i hora final lectura: %s\n", valores);
    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));

    //Valor màxim
    sprintf(sql, "SELECT MAX(Temperatura) FROM Lectures %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Valor màxim lectura: %s\n", valores);
    sprintf(maxim, "Valor màxim lectura: %s\n", valores) ;

    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));
    
    //Valor mínim
    sprintf(sql, "SELECT MIN(Temperatura) FROM Lectures %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Valor mínim lectura: %s\n", valores);
    sprintf(minim, "Valor mínim lectura: %s\n", valores) ;
    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));
    
    //Valor mig
    sprintf(sql, "SELECT AVG(Temperatura) FROM Lectures %s", buffer);
    sqlite3_exec(db, sql, callback, (void*)buffer, &zErrMsg);
    sprintf(text, "Valor mig lectura: %s\n", valores);
    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));


	rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	fclose(fp);
	sqlite3_close(db);
	//printf("maxim=%s",maxim);
	sprintf(text_email,"\nMail de prova\n\nSalutacions,\n\nToni Vives Cabaleiro.\nValors:%s\n%s",maxim,minim);
	email(nom_servidor, email_destinatari, email_remitent,text_email);

	return 0;
}

