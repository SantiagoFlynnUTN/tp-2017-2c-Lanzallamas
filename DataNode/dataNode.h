/*
 * dataNode.h
 *
 *  Created on: 10/9/2017
 *      Author: utnso
 */

#include <commons/config.h>
#include <commons/log.h>
#include <stdint.h>

#ifndef DATANODE_H_
#define DATANODE_H_

/* ESTRUCTURAS */

typedef struct {
	char ip[20];
	uint16_t puerto;
} ConexionFileSystem;

typedef struct {
	int nombreLen;
	char nombreNodo[100];
	char rutaDataBin[255];
	uint16_t puertoWorker;
} __attribute__((packed))
InfoNodo;

/* VARIABLES GLOBALES Y DEFINES */
#define ARCHIVO_CONFIGURACION "dataNode.conf"
#define ARCHIVO_LOGGER "dataNode.log"
#define MODULO "DATANODE"
#define IP_FILESYSTEM "IP_FILESYSTEM"
#define PUERTO_FILESYSTEM "PUERTO_FILESYSTEM"
#define NOMBRE_NODO "NOMBRE_NODO"
#define PUERTO_WORKER "PUERTO_WORKER"
#define RUTA_DATABIN "RUTA_DATABIN"
#define CHARSPORMB 1 // cambiar por la cantidad de chars que entran en 1MB

t_config * config;
t_log * logger;
ConexionFileSystem conexionFileSystem;
InfoNodo infoNodo;

/* FUNCIONES */
void inicializarDataNode();
void getBloque(int socket);
void setBloque(int socket);

#endif /* DATANODE_H_ */
