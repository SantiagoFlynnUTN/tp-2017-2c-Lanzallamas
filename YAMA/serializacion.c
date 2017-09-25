/*
 * seralizacion.c
 *
 *  Created on: 4/9/2017
 *      Author: utnso
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
#include <archivos.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sockets.h>
#include "serializacion.h"
#include "servidor.h"
#include "chat.h"
#include <protocoloComunicacion.h>
#include "yama.h"



void enviarTablaTransformacion(int socket_master){
	char nombArch[255];
	recv(socket_master, nombArch, sizeof(char)* 255, 0);
	int cantidadWorkersEjemplo = 2;

	if (send(socket_master, &cantidadWorkersEjemplo, sizeof(int), 0) == -1){
			printf("No se puedo enviar el mensaje.\n");
	}
	while(cantidadWorkersEjemplo--){
		operacionTransformacion op;
		strcpy(op.nombreNodo, "nodo1");
		op.bloque = cantidadWorkersEjemplo +2;
		op.bytes = 10;
		strcpy(op.ip, "127.0.0.1");
		op.puerto = htons(9002);
		strcpy(op.ruta, "temp/algo.txt");
		send(socket_master, &op, sizeof(op), 0);
	}

}

void manejarDatos(int buf, int socket){
	switch(buf){
	case SOLICITUDJOB:
		enviarTablaTransformacion(socket);
		break;
	}
}
