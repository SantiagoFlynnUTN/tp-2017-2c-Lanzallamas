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

int sockfd;

void enviarStructFileSystem(int socket){
	HandshakeNodo handshake;
	handshake.tipomensaje = 2;
	strcpy(handshake.nombreNodo, infoNodo.nombreNodo);

	struct sockaddr_in address;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	getpeername(socket, (struct sockaddr *)&address, &addr_size);
	memset(handshake.ip, 0, 20);
	strcpy(handshake.ip, inet_ntoa(address.sin_addr));
	handshake.puerto = infoNodo.puertoWorker;
	handshake.bloques = infoNodo.cantidadBloques;

	send(socket, &handshake, sizeof(handshake), 0);
}

int main(int argc, char *argv[]){
	inicializarDataNode();
	iniciarConexionAServer(&sockfd);
	//escribir_chat(sockfd);
	enviarStructFileSystem(sockfd);

	escuchar_chat(sockfd);

	for(;;);

	close(sockfd);
	return 0;
}