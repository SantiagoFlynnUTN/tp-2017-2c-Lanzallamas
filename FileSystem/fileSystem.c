/*
 * fileSystem.c
 *
 *  Created on: 9/9/2017
 *      Author: utnso
 */


#include <netinet/in.h>
#include "fileSystem.h"


void printNodo(DescriptorNodo a){
	printf("lenNombre: %d Nombre %s, direccion: %s:%u\n", a.nombreLen, a.nombreNodo, a.ip, a.puerto);
}

void asociarNodo(int socket){
	DescriptorNodo newNodo;
	recv(socket, &newNodo.nombreLen, sizeof(int), 0);
	recv(socket, newNodo.nombreNodo, 100 * sizeof(char), 0);
	recv(socket, newNodo.ip, 20 * sizeof(char), 0);
	recv(socket, &newNodo.puerto, sizeof(newNodo.puerto), 0);
	printNodo(newNodo);
}
