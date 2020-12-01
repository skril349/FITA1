
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

#include "funcmail.h"

#define REQUEST_MSG_SIZE 1024
#define REPLY_MSG_SIZE 500
#define SERVER_PORT_NUM 25


int main(int argc, char *argv[])
{

	char nom_servidor[32] = "172.20.0.21"; //Adreça IP per accedir al server
	char email_destinatari[REPLY_MSG_SIZE] = "1458570@campus.euss.org"; //Email del destinatari
	char email_remitent[REPLY_MSG_SIZE] = "afontquerni@euss.es"; //Email del remitent
	char text_email[REPLY_MSG_SIZE] = "\nMail de prova\n\nSalutacions,\n\nToni Vives Cabaleiro."; //Text del email.
	
	email(nom_servidor, email_destinatari, email_remitent, text_email);

	return 0;
}

