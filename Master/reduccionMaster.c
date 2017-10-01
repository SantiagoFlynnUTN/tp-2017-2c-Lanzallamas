/*
 * reduccionMaster.c
 *
 *  Created on: 1/10/2017
 *      Author: utnso
 */
#include <protocoloComunicacion.h>
#include "master.h"
#include <sys/types.h>
#include <sys/socket.h>

void solicitudReduccion(int socket_yama){

	int pedRed;
	pedRed = PEDIDOREDUCCION;
	if (send(socket_yama, &pedRed, sizeof(int), 0) ==-1)
						printf("No puedo enviar\n");
	operacionReduccion op;
	recv(socket_yama, &op, sizeof(op), 0);

	rutaArchivo rutas[op.cantidadTemporales];

	while (op.cantidadTemporales--) {
		recv(socket_yama, &rutas[op.cantidadTemporales], sizeof(rutaArchivo),
				0);
		printf("ruta: %s\n", &rutas[op.cantidadTemporales]);
	}
}
