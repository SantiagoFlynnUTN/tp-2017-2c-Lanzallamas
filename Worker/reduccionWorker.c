/*
 * reduccionWorker.c
 *
 *  Created on: 2/10/2017
 *      Author: utnso
 */

#include "worker.h"
#include <sockets.h>


void iniciarReduccion(int socket) {

	mensajeReduc opReduc;
	zrecv(socket, &opReduc, sizeof(opReduc), 0);

	int num = 4;
	zsend(socket, &num, sizeof(int), 0);
	exit(1);

}
