/*
 * protocoloComunicacion.h
 *
 *  Created on: 17/9/2017
 *      Author: utnso
 */

#ifndef PROTOCOLOCOMUNICACION_H_
#define PROTOCOLOCOMUNICACION_H_

typedef enum {PORTYAMA = 9000, PORTMASTER, PORTNODO, PORTFILESYSTEM} puertos;

typedef enum {SOLICITUDJOB=1, PEDIDOREDUCCION} ConexionesYAMAMaster;
//typedef enum {TRANSFORMACION=1} ConexionesMasterYAMA;
//typedef enum {OK=1} ConexionesFileSystemDataNode;
typedef enum {TRANSFORMACIONWORKER=1, REDUCLOCAL} ConexionesWorkerMaster;

#endif /* PROTOCOLOCOMUNICACION_H_ */
