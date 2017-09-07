/*
 * main.h
 *
 *  Created on: 3/9/2017
 *      Author: utnso
 */

#ifndef MAIN_H_
#define MAIN_H_

typedef struct {
	int tipoMensaje;
	char mensaje[100];
} __attribute__((packed))
mensajeCorto;

void nuevoCliente(char* remoteHost, int newfd);
void manejarDatos(int buf, int socket);

#endif /* MAIN_H_ */
