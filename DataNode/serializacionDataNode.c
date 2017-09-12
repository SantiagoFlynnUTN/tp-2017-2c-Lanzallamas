/*
 * serializacion.c
 *
 *  Created on: 12/9/2017
 *      Author: utnso
 */
#include "serializacionDataNode.h"
#include "conexionesDataNode.h"
#include "dataNode.h"

void manejarDatos(int buf, int socket){
	switch(buf){
	case GETBLOQUE:
		getBloque(socket);
		break;
	case SETBLOQUE:
		setBloque(socket);
		break;
	}
}
