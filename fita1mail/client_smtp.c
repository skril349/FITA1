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


#define REQUEST_MSG_SIZE 1024
#define REPLY_MSG_SIZE 500
#define SERVER_PORT_NUM 25

int email( char *nom_servidor, char *email_destinatari, char *email_remitent, char *text_email);



int main(int argc, char *argv[])
{

	/*char nom_servidor[32] = "172.20.0.21"; //Adreça IP per accedir al server
	char email_destinatari[REPLY_MSG_SIZE] = "1458570@campus.euss.org"; //Email del destinatari
	char email_remitent[REPLY_MSG_SIZE] = "afontquerni@euss.es"; //Email del remitent
	char text_email[REPLY_MSG_SIZE] = "\nMail de prova\n\nSalutacions,\n\nToni Vives Cabaleiro."; //Text del email.
	
	email(nom_servidor, email_destinatari, email_remitent, text_email);*/

while(1){
		char nom_servidor[32]; //Adreça IP per accedir al server
		char email_destinatari[REPLY_MSG_SIZE] ; //Email del destinatari
		char email_remitent[REPLY_MSG_SIZE] ; //Email del remitent
		//char text_email[REPLY_MSG_SIZE]; //Text del email.
		char text_email[REPLY_MSG_SIZE] = "\nMail de prova\n\nSalutacions,\n\nToni Vives Cabaleiro."; //Text del email.

		printf("Entri el nom del servidor al que es vol connectar: ");
		scanf("%s", &nom_servidor);
		printf("\n");
		printf("Entri el email del destinatari del correu:\n");
		scanf("%s", &email_destinatari);
		printf("\n");
		printf("Entri el email del remitent del correu:\n");
		scanf("%s", &email_remitent);
		printf("\n");
		//printf("Entri el text que vol enviar:\n");
		//scanf("%s", &text_email);
		//printf("\n");
		email(nom_servidor, email_destinatari, email_remitent, text_email);

	}

	return 0;
}

int email( char *nom_servidor, char *email_destinatari, char *email_remitent, char *text_email)
{
	struct sockaddr_in serverAddr;
	int sockAddrSize;
	int sFd;
	int result;
	char BufRead[REPLY_MSG_SIZE];
	char BufWrite[REPLY_MSG_SIZE];

	/*Crear el socket*/
	sFd=socket(AF_INET,SOCK_STREAM,0);
	if(sFd < 0)
	{
		perror("socket");
		exit(1);
	}

	/*Construir l'adreça*/
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); //Posar l'estructura a zero
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons (SERVER_PORT_NUM); //Posem el nostre port.
	serverAddr.sin_addr.s_addr = inet_addr(nom_servidor); //Posem l'adreça del servidor.

	/*Conexió*/
	result = connect (sFd, (struct sockaddr *) &serverAddr, sockAddrSize);
	if (result < 0)
	{
		printf("Error en establir la connexió\n");
		exit(-1);
	}
	printf("\nConnexió establerta amb el servidor: adreça %s, port %d\n",inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));


	//Presentació del servidor
	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	if (result < 0)
	{
		perror("Read presentacio:");
		exit(1);
	}
	printf ("Rebut(%d): %s\n", result, BufRead);

	//HELO
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "HELO Rpi27.iotlab.euss.es\n"); //Iniciem la conversació amb HELO.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));
	if (result < 0)
	{
		perror("Write");
		exit(1);
	}

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): -> %s\n", result, BufRead);


	//MAIL FROM
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "MAIL FROM: <%s>\n", email_remitent); //Enviem l'adreça d'email del remitent del correu.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);	
	
	
	//RCPT TO
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "RCPT TO: <%s>\n", email_destinatari); //Enviem l'adreça d'email del destinatari del correu.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);


	//DATA
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "DATA\n");
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);
	
	
	//Message
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "Subject: ADSTR\nFrom: %s\nTo: %s\n %s \n.\n", email_remitent, email_destinatari, text_email);
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);


	//QUIT
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "QUIT\n");
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);


	close(sFd);

	return 0;
}
