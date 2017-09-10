/*
 * main.h
 *
 *  Created on: 3/9/2017
 *      Author: utnso
 */

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>

#ifndef MAIN_H_
#define MAIN_H_

t_dictionary* nodos;

void nuevoCliente(char* remoteHost, int newfd);
void manejarDatos(int buf, int socket);

#endif /* MAIN_H_ */
