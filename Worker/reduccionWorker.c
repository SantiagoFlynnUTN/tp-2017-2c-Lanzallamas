/*
 * reduccionWorker.c
 *
 *  Created on: 2/10/2017
 *      Author: utnso
 */

#include "worker.h"

void iniciarReduccion(int socket) {

	mensajeReduc opReduc;
	recv(socket, &opReduc, sizeof(opReduc), 0);

	int num = 4;
	send(socket, &num, sizeof(int), 0);
	exit(1);

}
