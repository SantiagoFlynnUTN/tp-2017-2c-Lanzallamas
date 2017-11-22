#include "serializacionDataNode.h"
#include "conexionesDataNode.h"
#include "dataNode.h"
#include "operacionesDataNode.h"
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <structUtiles.h>
#include <protocoloComunicacion.h>
#include <sockets.h>

void _getBloque(int socket);
void _setBloque(int socket);

void manejarDatos(int buf, int socket){
	switch(buf){
		case GETBLOQUE:
			_getBloque(socket);
			break;
		case SETBLOQUE:
			_setBloque(socket);
			break;
	}
}

void _getBloque(int socket){
	int bloque, status;
	char data[MB];
	zrecv(socket, &bloque, sizeof(bloque), 0);

	status = getBloque(bloque, data);

	if(status == -1){
		log_error(logger, "Hubo un error leyendo el bloque\n");
	}

	if(send(socket, data, sizeof(char) * MB, 0) == -1){
		log_error(logger, "No se pudo enviar el mensaje.\n");
	}
}

void _setBloque(int socket){
	int bloque, status,mensaje;
	char data[MB];

	memset(data, 0, MB);

	zrecv(socket, &bloque, sizeof(bloque), 0);
	zrecv(socket, data, sizeof(char) * MB, 0);

	status = setBloque(bloque, data);

	if(status == -1){
		mensaje = DATANODEERROR;
		log_error(logger, "Hubo un error guardando el bloque\n");
	}else{
		mensaje = DATANODEOK;
	}

	zsend(socket, &mensaje, sizeof(mensaje), 0);
}
