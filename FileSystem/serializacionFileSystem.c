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
#include <archivos.h>
#include "servidor.h"
#include "chat.h"
#include "conexionesFileSystem.h"
#include "fileSystem.h"
#include "serializacionFileSystem.h"
#include <sockets.h>
#include "funcionesConsolaFS.h"

void recibirArchivo(int socket, char * ruta);

void manejarStructs(int socket){
	//por ahora manejamos solo mensajes
	leerMensaje(socket);
}

void manejarDatos(int buf, int socket){
	switch(buf){
	case OK:
		log_debug(logger, "Socket %i dice OK\n", socket);
		break;
	case CONEXIONNODO:
		asociarNodo(socket);
		break;
	case SOLICITUDARCHIVOYAMA:
		responderYAMA(socket);
		break;
    case RECEPCIONBLOQUE:
		recibirArchivo(socket, "prueba1234.txt");
        break;
    case RECEPCIONARCHIVOWORKER:
    	break;
	case ERRORGUARDARBLOQUE:
		break;
	}
}

void recibirArchivo(int socket, char * ruta){
	int longitud;
	char archivoFS[255];

	zrecv(socket, archivoFS, sizeof(char) * 255, 0);
	zrecv(socket, &longitud, sizeof(longitud), 0);

	char buffer[longitud];
	memset(buffer, 0, longitud);

	zrecv(socket, &buffer, longitud * sizeof(char), 0);

	guardarArchivo(ruta, buffer, longitud);
	int respuesta = cpfrom("prueba1234.txt", archivoFS);

	zsend(socket, &respuesta, sizeof(respuesta), 0);
}
