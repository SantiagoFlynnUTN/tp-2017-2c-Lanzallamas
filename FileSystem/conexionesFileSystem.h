/*
 * conexiones.h
 *
 *  Created on: 3/9/2017
 *      Author: utnso
 */

#ifndef CONEXIONESFILESYSTEM_H_
#define CONEXIONESFILESYSTEM_H_

void responderYAMA(int socketYAMA);

enum tipoMensaje {OK = 1, CONEXIONNODO, SOLICITUDARCHIVOYAMA, RECEPCIONBLOQUE, RECEPCIONARCHIVOWORKER, ERRORGUARDARBLOQUE};
enum tipoMensajeEnviar {GETBLOQUE=1, SETBLOQUE};

#endif /* CONEXIONESFILESYSTEM_H_ */
