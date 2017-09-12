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
#include <prueba.h>
#include "servidor.h"
#include "mainFS.h"
#include "chat.h"
#include "conexionesFileSystem.h"
#include "fileSystem.h"
#include "serializacionFileSystem.h"

#define PORT 9034 // puerto al que vamos a conectar

#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez

int yama_socket;


void nuevoCliente(char* remoteHost, int newfd){
	printf("new conection from %s on socket %d\n", remoteHost, newfd);
	//Y acá hacer algo con el nuevo cliente conectado
}

void pruebaZCommons(){
	myfunc();
}

int main(int argc, char *argv[]){
	pruebaZCommons();
	inicializarFileSystem();
	inicializarServer();

	for(;;);
}
