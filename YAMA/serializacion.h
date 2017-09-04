/*
 * serializacion.h
 *
 *  Created on: 4/9/2017
 *      Author: utnso
 */

#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

enum tipoProceso {YAMA, FILESYSTEM, DATANODE, MASTER, WORKER};

void manejarDatos(int buf, int socket);

#endif /* SERIALIZACION_H_ */
