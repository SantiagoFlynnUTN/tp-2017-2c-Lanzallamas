/*
 * conexiones.c
 *
 *  Created on: 15/9/2017
 *      Author: utnso
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "conexionesFileSystem.h"
#include "mainFS.h"
#include "fileSystem.h"
#include "errno.h"
#include "commons/string.h"


void format(){
	//A desarrollar
}

void rm(char* path_archivo){
	//A desarrollar (-d y -b)
}

void rename(char* path_original, char nombre_final){

}

void mv(char* path_original, char* path_final){

}

void cat(char* path_archivo){

}

int mkdir(char* path_dir){

	log_debug(logger, "accediendo poke_mkdir: %s", path_dir);

		char *dupPath = strdup(path_dir);
		char *fname = getNombreFile(dupPath);
		free(dupPath);

		//Verifico que el nombre no sea mas largo de lo permitido
		if(string_length(fname) > MAX_LENGTH){
			log_error(logger, "El nombre del directorio es mas largo del permitido");
			return -ENAMETOOLONG;
		}

		//Verifico que el file no exista en la tabla de directorios
		int existe = 1;
		int i = 0;

		if(!existe == 0){

			while(!tabla_Directorios[i].nombre == fname){
				i++;
			}
		//Obtengo nombre del padre y busco su index en la tabla
		}
}

void cpfrom(char* path_archivo_origen, char* directorio_yamafs){

}

void cpto(char* path_archivo_yamafs, char* directorio_filesystem){

}

void cpblock(char* path_archivo, int nro_bloque, int id_nodo){

}

void md5(char* path_archivo_yamafs){

}

void ls(char* path_directorio){

}

void info(char* path_archivo){

}



//FUNCIONES AUXILIARES//

char *getNombreFile(char *path){

	char *nombreFile = string_new();
	char** subpath = string_split(path, "/");

	void closure(char *value){
		free(nombreFile);
		nombreFile = value;
	}

	string_iterate_lines(subpath, closure);

	free(subpath);

	if(string_ends_with(nombreFile, "/")){

		char *subnombreFile = string_substring_until(nombreFile, string_length(nombreFile) - 1);
		free(nombreFile);
		nombreFile = subnombreFile;
	}

	return nombreFile;
}



