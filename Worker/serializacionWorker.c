/*
 * serializacionWorker.c
 *
 *  Created on: 12/9/2017
 *      Author: utnso
 */
#include "serializacionWorker.h"
#include "conexionesWorker.h"

void manejarDatos(int buf, int socket){
	switch(buf){
	case TRANSFORMACION:
		break;
	case REDUCCIONLOCAL:
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
