/*
 * worker.h
 *
 *  Created on: 11/9/2017
 *      Author: utnso
 */

#include <commons/config.h>
#include <commons/log.h>
#include <stdint.h>

#ifndef WORKER_H_
#define WORKER_H_


/* ESTRUCTURAS */

typedef struct {
	int handshake;
	pid_t parentPid;
	pid_t childPid;
}Handshake;

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

typedef struct{
	int cantidadBytes;
	int bloque;
	char nombreTemp[255];
}mensajeTransf;

/* VARIABLES GLOBALES Y DEFINES */
#define ARCHIVO_CONFIGURACION "worker.conf"
#define ARCHIVO_LOGGER "worker.log"
#define MODULO "WORKER"
#define IP_FILESYSTEM "IP_FILESYSTEM"
#define PUERTO_FILESYSTEM "PUERTO_FILESYSTEM"
#define NOMBRE_NODO "NOMBRE_NODO"
#define RUTA_DATABIN "RUTA_DATABIN"

t_config * config;
t_log * logger;
ConexionFileSystem conexionFileSystem;
InfoNodo infoNodo;

/* FUNCIONES */
void inicializarWorker();

#endif /* WORKER_H_ */
