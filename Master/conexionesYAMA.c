/*
 * conexionesYAMA.c
 *
 *  Created on: 17/9/2017
 *      Author: utnso
 */

#include <protocoloComunicacion.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "conexionesYAMA.h"
#include "mainMaster.h"

void solicitudJob(int socket, char * archivoTransformar){
	SolicitudJob sol;
	sol.tipoMensaje = 1;
	memset(sol.rutaArchivo, 0, 255);
	strcpy(sol.rutaArchivo, archivoTransformar);
	//sol.rutaArchivo = ...
	if (send(socket, &sol, sizeof(SolicitudJob), 0) == -1){
		printf("No se puedo enviar el mensaje.\n");
	}
}
