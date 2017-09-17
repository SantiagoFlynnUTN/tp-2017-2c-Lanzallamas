/*
 * archivos.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "archivos.h"
#include <stdio.h>

bool verificarExistenciaDeArchivo(char* path) {
	FILE * archivoConfig = fopen(path, "r");
	if (archivoConfig!=NULL){
		 fclose(archivoConfig);
		 return true;
	}
	return false;
}

void guardarArchivo(char * path, char * datos, int longitud){
	FILE * archivo = fopen(path, "w");

	fwrite(datos, sizeof(*datos) * longitud, 1, archivo);

	fclose(archivo);
}
