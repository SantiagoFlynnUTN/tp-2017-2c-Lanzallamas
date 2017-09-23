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
#include "conexionesWorker.h"
#include <sys/wait.h>
#include "cliente.h"

void recibirArchivo(int socket, char * ruta){
	int longitud;
	recv(socket, &longitud, sizeof(longitud), 0);
	char buffer[longitud];
	memset(buffer, 0, longitud);
	int a = recv(socket, &buffer, longitud, 0);

	guardarArchivo(ruta, buffer, longitud);
}

void iniciarTransformacion(int socket){
	mensajeTransf t;
	recv(socket, &t, sizeof(t), 0);
	char * ruta = (char *)malloc(sizeof(char) * 255);
	sprintf(ruta, "scripts/transformacion%d.sh", getpid());
	recibirArchivo(socket, ruta);
	char * command =  (char *)malloc(sizeof(char) * 255);
	sprintf(command, "chmod 777 %s && ./%s\n", ruta, ruta);
	printf(command);
	system(command);

	int num = 4;
	send(socket, &num, sizeof(int), 0);
	free(ruta);
	free(command);
	exit(1);
}


