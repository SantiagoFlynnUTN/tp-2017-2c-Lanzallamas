/*
 * master.h
 *
 *  Created on: 9/9/2017
 *      Author: utnso
 */
#include <commons/config.h>
#include <stdint.h>

#ifndef MASTER_H_
#define MASTER_H_

/* ESTRUCTURAS */
typedef struct {
	char ip[20];
	uint16_t puerto;
} YamaConexion;

/* VARIABLES GLOBALES Y DEFINES */
#define ARCHIVOCFG "master.conf"
#define YAMA_IP "YAMA_IP"
#define YAMA_PUERTO "YAMA_PUERTO"

YamaConexion conexionYAMA;
t_config * config;

/* FUNCIONES */
void inicializarMaster();


#endif /* MASTER_H_ */
