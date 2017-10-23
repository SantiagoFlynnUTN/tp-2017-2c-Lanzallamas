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
	zsend(socket, &sol.tipoMensaje, sizeof(sol.tipoMensaje), 0);
	zsend(socket, &sol.rutaArchivo, sizeof(sol.rutaArchivo), 0);
}
