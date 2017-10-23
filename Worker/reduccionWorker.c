#include "worker.h"
#include <sockets.h>
#include <protocoloComunicacion.h>


void iniciarReduccion(int socket) {

	mensajeReduc opReduc;
	zrecv(socket, &opReduc, sizeof(opReduc), 0);

	//rutinaTransformacion
	int status = 0;

	zsend(socket, &status, sizeof(status), 0);
	exit(1);

}
