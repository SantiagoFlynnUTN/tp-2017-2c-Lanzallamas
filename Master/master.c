/*
 * master.c
 *
 *  Created on: 10/9/2017
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "master.h"

void _cargarConfiguracion();
void _copiarYAMAIP();

void inicializarMaster(){
	_cargarConfiguracion();
}

void _cargarConfiguracion(){
	config = config_create(ARCHIVOCFG);
	if (!config_has_property(config, YAMA_IP) || !config_has_property(config, YAMA_PUERTO)){
		printf("badConfig 120");
		exit(120);
	}

	conexionYAMA.puerto = htons(config_get_int_value(config, YAMA_PUERTO));
	_copiarYAMAIP();
}

void _copiarYAMAIP(){
	char * ip;

	ip = config_get_string_value(config, YAMA_IP);

	int i = 0;

	// Copio el valor caracter a caracter porque ip es un char[20] y la config devuelve char *
	while(*ip != '\0'){
		conexionYAMA.ip[i] = *ip;
		ip++;
		i++;
	}
}
