/*
 * master.h
 *
 *  Created on: 9/9/2017
 *      Author: utnso
 */
#include <commons/config.h>
#include <commons/log.h>
#include <stdint.h>

#ifndef MASTER_H_
#define MASTER_H_

/* ESTRUCTURAS */

typedef struct {
	pid_t parentPid;
	pid_t childPid;
}Handshake;

typedef struct{
	int transformacion;
	int cantidadWorkers;
}Transformacion;

typedef struct {
	char ip[20];
	uint16_t puerto;
} YamaConexion;

typedef struct {
	int tipoMensaje;
	char rutaArchivo[100];
} SolicitudJob;

typedef struct {
	char nombreNodo[100];
	char ipWorker[20];
	uint16_t puertoWorker;
	int numBloque;
	int bytesOcupados;
	char rutaArchivo[100];
} __attribute__((packed))
workerTransformacion;

/* VARIABLES GLOBALES Y DEFINES */
#define ARCHIVOCFG "master.conf"
#define ARCHIVO_LOGGER "master.log"
#define MODULO "MASTER"
#define YAMA_IP "YAMA_IP"
#define YAMA_PUERTO "YAMA_PUERTO"


YamaConexion conexionYAMA;
t_log * logger;
t_config * config;

/* FUNCIONES */
void inicializarMaster();


#endif /* MASTER_H_ */
