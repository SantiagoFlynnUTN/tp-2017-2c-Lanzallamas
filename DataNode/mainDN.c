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
#include "cliente.h"
#include "dataNode.h"

#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez

#define OK 1
#define ESTRUCTURA 2
#define ARCHIVO 3

typedef struct t_handshake{
	int tipomensaje;
	int nombreLen;
	char nombreNodo[100];
	char ip[20];
	uint16_t puerto;
} __attribute__((packed))
HandshakeNodo;

int sockfd;

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

void enviarStructFileSystem(int socket){
	HandshakeNodo handshake;
	handshake.tipomensaje = 2;
	handshake.nombreLen = infoNodo.nombreLen;
	strcpy(handshake.nombreNodo, infoNodo.nombreNodo);

	struct sockaddr_in address;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	getpeername(socket, (struct sockaddr *)&address, &addr_size);
	memset(handshake.ip, 0, 20);
	strcpy(handshake.ip, inet_ntoa(address.sin_addr));
	handshake.puerto = infoNodo.puerto;

	send(socket, &handshake, sizeof(handshake), 0);
}

int main(int argc, char *argv[]){
	inicializarDataNode();
	iniciarConexionAServer(&sockfd);
	//escribir_chat(sockfd);
	enviarStructFileSystem(sockfd);

	//escuchar_chat(sockfd);

	for(;;);

	close(sockfd);
	return 0;
}
