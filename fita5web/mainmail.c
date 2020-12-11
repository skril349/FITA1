
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
#include <getopt.h>
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
	char email_destinatari[REPLY_MSG_SIZE] = "1396920@campus.euss.org"; //Email del destinatari
	char email_remitent[REPLY_MSG_SIZE] = "afontquerni@euss.es"; //Email del remitent
	char text_email[REPLY_MSG_SIZE]; //Text del email.
	char basedades[REPLY_MSG_SIZE] = "/home/pi/Desktop/FITA1/fita5web/temperatures.db"; //Nom de la base de dades
	char minim[1024];
	char maxim[1024];
	char minimSpi[1024];
	char maximSpi[1024];
	sqlite3 *db;
	char *zErrMsg = 0;
	char buffer[1024];
	char text[1024];
	char sql[1024];
	memset(buffer, '\0', 1024);
	int rc;

	int opt= 0;
    static struct option long_options[] = 
    {
        {"servidor", required_argument, 0, 's'},
        {"origen", required_argument, 0, 'o'},
        {"desti", required_argument, 0, 'd'},
        {"basedades", required_argument, 0, 'b'},
        {0, 0, 0, 0}
    };

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"s:o:d:b:", long_options, &long_index )) != -1) 
    {
        switch (opt) 
        {

             case 's' : strcpy(nom_servidor, optarg); 
                 break;
             case 'o' : strcpy(email_remitent, optarg);
                 break;
             case 'd' : strcpy(email_destinatari, optarg);
                 break;
             case 'b' : strcpy(basedades, optarg);
                 break;
             default: 	error(); 
                 exit(EXIT_FAILURE);
        }
    }
    
    printf("Aquest codi està generant un email accedint des del terminal on les dades estàn organitzades de la següent manera:\n");
    printf(" ./mainmail -s 172.20.0.21 -o lalala@euss.es -d 1396920@campus.euss.org -b /home/pi/Desktop/FITA1/fita5web/temperatures.db\n");
    //delay(5000);
    system("cls");
    printf("Servidor: %s, Destinatari: %s, Remitent: %s.\n", nom_servidor, email_destinatari, email_remitent);

	
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
    sprintf(sql, "SELECT MIN(Temps) FROM Lectures WHERE Id = 81 %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Data i hora inici lectura: %s\n", valores) ;

    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));
 
    //Data i hora final
    sprintf(sql, "SELECT MAX(Temps) FROM Lectures WHERE Id = 81 %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Data i hora final lectura: %s\n", valores);
    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));

    //Valor màxim
    sprintf(sql, "SELECT MAX(Temperatura) FROM Lectures WHERE Id = 81 %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Valor màxim lectura: %s\n", valores);
    sprintf(maxim, "Valor màxim lectura: %s\n", valores) ;

    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));
    
    //Valor mínim
    sprintf(sql, "SELECT MIN(Temperatura) FROM Lectures WHERE Id = 81 %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Valor mínim lectura: %s\n", valores);
    sprintf(minim, "Valor mínim lectura: %s\n", valores) ;
    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));
    
    //Valor mig
    printf(buffer);
    sprintf(sql, "SELECT AVG(Temperatura) FROM Lectures WHERE Id = 81 %s", buffer);
    sqlite3_exec(db, sql, callback, (void*)buffer, &zErrMsg);
    sprintf(text, "Valor mig lectura: %s\n", valores);
    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));


//-----------------------------

    //Data i hora inicial
    sprintf(sql, "SELECT MIN(Temps) FROM Lectures WHERE Id = 82 %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Data i hora inici lectura: %s\n", valores) ;

    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));
 
    //Data i hora final
    sprintf(sql, "SELECT MAX(Temps) FROM Lectures WHERE Id = 82 %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Data i hora final lectura: %s\n", valores);
    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));

    //Valor màxim
    sprintf(sql, "SELECT MAX(Temperatura) FROM Lectures WHERE Id = 82 %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Valor màxim lectura: %s\n", valores);
    sprintf(maximSpi, "Valor màxim lectura: %s\n", valores) ;

    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));
    
    //Valor mínim
    sprintf(sql, "SELECT MIN(Temperatura) FROM Lectures WHERE Id = 82 %s", buffer);
    sqlite3_exec(db, sql, callback, (void *)buffer, &zErrMsg);
    sprintf(text, "Valor mínim lectura: %s\n", valores);
    sprintf(minimSpi, "Valor mínim lectura: %s\n", valores) ;
    fprintf(fp, "%s", text);
    memset(text, '\0', sizeof(text));
    
    //Valor mig
    printf(buffer);
    sprintf(sql, "SELECT AVG(Temperatura) FROM Lectures WHERE Id = 82 %s", buffer);
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
	sprintf(text_email,"\nMail de prova\n\nSalutacions,\n\nToni Vives Cabaleiro.\nValors:%s\n%s\n%s\n%s",maxim,minim,maximSpi,minimSpi);
	email(nom_servidor, email_destinatari, email_remitent,text_email);

	return 0;
}

