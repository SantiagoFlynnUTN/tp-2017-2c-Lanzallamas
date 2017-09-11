/*
 * yama.c
 *
 *  Created on: 10/9/2017
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "yama.h"

void _cargarConfiguracion();
void _copiarFileSystemIP();
void _definirAlgoritmoBalanceo();

void inicializarYAMA(){
	_cargarConfiguracion();
}

void _cargarConfiguracion(){
	config = config_create(ARCHIVO_CONFIGURACION);
	if (!config_has_property(config, FS_IP) ||
		!config_has_property(config, FS_PUERTO) ||
		!config_has_property(config, RETARDO_PLANIFICACION) ||
		!config_has_property(config, ALGORITMO_BALANCEO)){
		printf("badConfig 120");
		exit(120);
	}

	conexionFileSystem.puerto = htons(config_get_int_value(config, FS_PUERTO));
	retardoPlanificacion = config_get_int_value(config, RETARDO_PLANIFICACION);
	_definirAlgoritmoBalanceo();
	_copiarFileSystemIP();
}

void _copiarFileSystemIP(){
	char * ip;

	ip = config_get_string_value(config, FS_IP);

	int i = 0;

	// Copio el valor caracter a caracter porque ip es un char[20] y la config devuelve char *
	while(*ip != '\0'){
		conexionFileSystem.ip[i] = *ip;
		ip++;
		i++;
	}
}

void _definirAlgoritmoBalanceo(){
	char * algoritmo;

	algoritmo = config_get_string_value(config, ALGORITMO_BALANCEO);

	if(strcmp(algoritmo,ROUND_ROBIN) == 0){
		algoritmoBalanceo = ROUNDROBIN;
	} else if(strcmp(algoritmo,WEIGHTED_ROUND_ROBIN) == 0){
		algoritmoBalanceo = WEIGHTEDROUNDROBIN;
	} else {
		printf("ALGORITMO DE PLANIFICACION DESCONOCIDO\n");
		exit(121);
	}
}
