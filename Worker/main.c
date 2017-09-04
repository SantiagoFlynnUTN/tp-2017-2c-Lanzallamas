/*
    ** client.c -- Ejemplo de cliente de sockets de flujo
    */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "chat.h"

#define PORT 9034 // puerto al que vamos a conectar

#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez

#define OK 1
#define ESTRUCTURA 2
#define ARCHIVO 3

int sockfd;

//https://es.wikibooks.org/wiki/Programaci%C3%B3n_en_C/Estructuras_y_Uniones
typedef struct {
	int tipoMensaje;
	char mensaje[100];
} __attribute__((packed))
mensajeCorto;


int funcionTest(){
	int salir = 1;
	int numbytes;
	char buf[MAXDATASIZE];

	while(salir){
		if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
				perror("recv");
				exit(1);
			}
		buf[numbytes] = '\0';

		printf("Received: %s",buf);

		if (!strncmp(buf, "chau", 4)){

			if (send(sockfd, "Me llego chau\n", 13, 0) == -1)
					perror("send");

			close(sockfd);
			salir = 0;

		}
		else{
			if (send(sockfd, "Me llego inv\n", 12, 0) == -1)
								perror("send");
		}
	}
	return 0;
}

void manejarDatos(int buf, int socket){
	switch(buf){
	case OK:
		printf("socket %i dice OK\n", socket);
		break;
	case ESTRUCTURA:
		//por ahora manejamos solo mensajes
		leerMensaje(socket);
		break;
	case ARCHIVO:
		printf("crear funcion para deserializar archivo\n");
		break;
	}
}


int main(int argc, char *argv[])
{

	struct sockaddr_in their_addr; // información de la dirección de destino


	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
	their_addr.sin_port = htons(PORT);  // short, Ordenación de bytes de la red
	their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(their_addr.sin_zero), 0, 8);  // poner a cero el resto de la estructura

	if (connect(sockfd, (struct sockaddr *)&their_addr,
										  sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}

	escribir_chat(sockfd);
	//escuchar_chat(sockfd);

	//funcionTest();
	for(;;);

	close(sockfd);
	return 0;
}
