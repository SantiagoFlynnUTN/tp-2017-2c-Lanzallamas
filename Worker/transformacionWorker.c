/*
 * transformacion.c
 *
 *  Created on: 17/9/2017
 *      Author: utnso
 */

#include "transformacionWorker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <archivos.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "chat.h"
#include "worker.h"
#include <sys/wait.h>
#include "cliente.h"
#include <sockets.h>

void recibirArchivo(int socket, char * ruta){
	int longitud;
	zrecv(socket, &longitud, sizeof(longitud), 0);
	char buffer[longitud];
	memset(buffer, 0, longitud);
	printf("entre\n");
	zrecv(socket, &buffer, longitud * sizeof(char), 0);
	printf("sali\n");
	//guardarArchivo(ruta, buffer, longitud);
}

void iniciarTransformacion(int socket){
	mensajeTransf t;
	zrecv(socket, &t, sizeof(t), 0);
	char * ruta = (char *)malloc(sizeof(char) * 255);
	sprintf(ruta, "scripts/transformacion%d.sh", getpid());
	recibirArchivo(socket, ruta);
	char * command =  (char *)malloc(sizeof(char) * 255);
	sprintf(command, "./%s\n", ruta);
	printf(command);
	//system(command);
	printf("bloque: %d\n", t.bloque);
	int num = 4;
	zsend(socket, &num, sizeof(int), 0);
	free(ruta);
	free(command);
	exit(1);
}


