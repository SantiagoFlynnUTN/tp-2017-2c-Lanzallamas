/*
    ** client.c -- Ejemplo de cliente de sockets de flujo
    */

#include "mainFS.h"

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
#include "servidor.h"
#include "chat.h"
#include "conexiones.h"
#include "serializacion.h"
#include "fileSystem.h"

#define PORT 9034 // puerto al que vamos a conectar

#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez

int yama_socket;


void nuevoCliente(char* remoteHost, int newfd){
	printf("new conection from %s on socket %d\n", remoteHost, newfd);
	//Y acá hacer algo con el nuevo cliente conectado
}



int main(int argc, char *argv[]){
	inicializarFileSystem();
	inicializarServer();

	for(;;);
}
