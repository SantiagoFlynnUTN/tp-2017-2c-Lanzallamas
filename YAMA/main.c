/*
 * main.c
 *
 *  Created on: 2/9/2017
 *      Author: utnso
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "conexiones.h"

void manejarDatos(int buffer, int socket){
	switch(buffer){
	case OK:
		printf("socket %i dice OK\n", socket);
		break;
	case ESTRUCTURA:
		printf("crear funcion para deserializar struct\n");
		break;
	case ARCHIVO:
		printf("crear funcion para deserializar archivo\n");
		break;
	}
}

void nuevoCliente(char* remoteHost, int newfd){
	printf("new conection from %s on socket %d\n", remoteHost, newfd);
	//Y acá hacer algo con el nuevo cliente conectado
}


void conectarAFileSystem(){

	for(;;);
}

int main(){

	inicializarServer();
	conectarAFileSystem();

	return 0;
}
