/*
 * conexiones.h
 *
 *  Created on: 3/9/2017
 *      Author: utnso
 */

#ifndef CONEXIONES_H_
#define CONEXIONES_H_

enum tipoMensaje {OK = 1, ESTRUCTURA, ARCHIVO};

#define PORT 9034   // puerto en el que escuchamos
#define MAXCLIENTES 100

void inicializarServer();

#endif /* CONEXIONES_H_ */
