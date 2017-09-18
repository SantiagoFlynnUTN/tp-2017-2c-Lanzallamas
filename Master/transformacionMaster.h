/*
 * transformacionMaster.h
 *
 *  Created on: 18/9/2017
 *      Author: utnso
 */

#ifndef TRANSFORMACIONMASTER_H_
#define TRANSFORMACIONMASTER_H_

int respuestaSolicitud(int socket_yama);
void mandarTransformacionNodo(int socket_nodo, int cantidadWorkers);
void atenderConexion(int socket);

#endif /* TRANSFORMACIONMASTER_H_ */
