/*
 * servidor.h
 *
 *  Created on: 4/9/2017
 *      Author: utnso
 */

#ifndef SERVIDOR_H_
#define SERVIDOR_H_

enum tipoMensaje {OK = 1, ESTRUCTURA, ARCHIVO};

#define PORT 9034   // puerto en el que escuchamos

void inicializarServer();

#endif /* SERVIDOR_H_ */
