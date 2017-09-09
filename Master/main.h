/*
 * main.h
 *
 *  Created on: 3/9/2017
 *      Author: utnso
 */
#include "master.h"
#ifndef MAIN_H_
#define MAIN_H_

void nuevoCliente(char* remoteHost, int newfd);
void manejarDatos(int buf, int socket);

YamaConexion conexion;

#endif /* MAIN_H_ */
