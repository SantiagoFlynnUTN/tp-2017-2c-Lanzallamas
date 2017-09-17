/*
 * servidor.h
 *
 *  Created on: 4/9/2017
 *      Author: utnso
 */

#ifndef SERVIDORMASTER_H_
#define SERVIDORMASTER_H_

enum tipoMensaje {OK = 1, ESTRUCTURA, ARCHIVO};

#define PORT 9034   // puerto en el que escuchamos

void inicializarServer();
void manejarCliente(int socket);

#endif /* SERVIDORMASTER_H_ */
