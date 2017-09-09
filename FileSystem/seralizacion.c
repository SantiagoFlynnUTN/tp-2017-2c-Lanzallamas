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
#include <arpa/inet.h>
#include <pthread.h>
#include "serializacion.h"
#include "servidor.h"
#include "chat.h"
#include "conexiones.h"
#include "fileSystem.h"



void manejarStructs(int socket){
	//por ahora manejamos solo mensajes
	leerMensaje(socket);
}

void manejarDatos(int buf, int socket){
	switch(buf){
	case OK:
		printf("socket %i dice OK\n", socket);
		break;
	case CONEXIONNODO:
		asociarNodo(socket);
		break;
	case CONEXIONYAMA:
		break;
	case SOLICITUDARCHIVOYAMA:
		break;
	case ERRORGUARDARBLOQUE:
		break;
	}
}
