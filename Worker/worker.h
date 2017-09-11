/*
 * worker.h
 *
 *  Created on: 11/9/2017
 *      Author: utnso
 */

#include <commons/config.h>
#include <stdint.h>

#ifndef WORKER_H_
#define WORKER_H_


/* ESTRUCTURAS */

typedef struct {
	char ip[20];
	uint16_t puerto;
} ConexionFileSystem;

typedef struct {
	int nombreLen;
	char nombreNodo[100];
	char rutaDataBin[255];
	uint16_t puerto;
} __attribute__((packed))
InfoNodo;

/* VARIABLES GLOBALES Y DEFINES */
#define ARCHIVO_CONFIGURACION "worker.conf"
#define IP_FILESYSTEM "IP_FILESYSTEM"
#define PUERTO_FILESYSTEM "PUERTO_FILESYSTEM"
#define NOMBRE_NODO "NOMBRE_NODO"
#define PUERTO_WORKER "PUERTO_WORKER"
#define RUTA_DATABIN "RUTA_DATABIN"

t_config * config;
ConexionFileSystem conexionFileSystem;
InfoNodo infoNodo;

/* FUNCIONES */
void inicializarWorker();

#endif /* WORKER_H_ */
