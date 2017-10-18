/*
 * reduccionWorker.c
 *
 *  Created on: 2/10/2017
 *      Author: utnso
 */

#include "worker.h"
#include <sockets.h>
#include <protocoloComunicacion.h>


void iniciarReduccion(int socket) {

	mensajeReduc opReduc;
	zrecv(socket, &opReduc, sizeof(opReduc), 0);

	//rutinaTransformacion

	zsend(socket, TRANSFORMACIONOK, sizeof(int), 0);
	exit(1);

}
