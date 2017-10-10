/*
 * serializacionWorker.c
 *
 *  Created on: 12/9/2017
 *      Author: utnso
 */
#include "serializacionWorker.h"
#include "transformacionWorker.h"
#include "reduccionWorker.h"
#include <protocoloComunicacion.h>

void manejarDatos(int buf, int socket){
	switch(buf){
	case TRANSFORMACIONWORKER:
		iniciarTransformacion(socket);
		break;
	case REDUCLOCAL:
		iniciarReduccion(socket);
		break;
	case REDUCCIONGLOBAL:
		break;
	case ALMACENAMIENTO:
		break;
    case SOLICITUDARCHIVOWORKER:
        break;
    case RECEPCIONARCHIVOWORKER:
    	break;
	case FILESYSTEMOK:
		break;
	case FILESYSTEMERROR:
		break;
	}
}
