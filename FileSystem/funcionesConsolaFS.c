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
#include "funcionesConsolaFS.h"
#include "commons/collections/queue.h"
#include <readline/readline.h>


void hiloConsola(){
	printf("holamundo! soy un hilo gay\n");
	while(1){

		char * linea;
		linea = readline(">");

		if (!linea) {
			break;
		}
		printf("%s\n", linea);
		free(linea);

	}
	pthread_exit(NULL);
}


void crear_hilo_consola(){
	int rc;
	pthread_t tid;
	rc = pthread_create(&tid, NULL, hiloConsola, NULL);
	if(rc) printf("no pudo crear el hilo");
}




/*
void format(){
	//A desarrollar
}
void rm(char* path_archivo){
	//A desarrollar (-d y -b)
}*/


//void yama_rename(char* path_original, char nombre_final){


//}

void mv(char* path_original, char* path_final){

}

void cat(char* path_archivo){

}

int yama_mkdir(char* path_dir){

	log_debug(logger, "accediendo mkdir: %s", path_dir);

	char *dupPath = strdup(path_dir);
	char *fname = yamaGetNFile(dupPath);
	free(dupPath);

	//Verifico que el nombre no sea mas largo de lo permitido
	if(string_length(fname) > MAX_LENGTH){
		log_error(logger, "El nombre del directorio es mas largo del permitido");
		return -ENAMETOOLONG;
	}

	//Verifico que el file no exista en la tabla de directorios
	int existe = 0;
	int i = 0;

	if(existe == 0){

		while(!tabla_Directorios[i].nombre == fname){
			i++;
		}
		existe = 1;
		//enviar mensaje a quien corresponda de que el directorio ya existe
	}

	//Obtengo el nombre del padre
	char *dpath = yamaGetPathPadre(path_dir);
	dupPath = strdup(dpath);
	char *dname = yamaGetNFile(dupPath);
	free(dupPath);
	//Obtener datos padre
	existe = 0;
	int p = 0;
	if(existe == 0){

		while(!tabla_Directorios[p].nombre == dname){
			p++;
		}
		existe = 1;
	}
	//Creo el nuevo Directorio y asigno a la tabla
	//Chequear si hay espacio en la tabla?
	Directorio newDir = yamaCrearDirectorio(i, fname, tabla_Directorios[p].id);
	asignarEspacioEnTabla(newDir);

	return 0;
}

void cpfrom(char* path_archivo_origen, char* directorio_yamafs){

}

void cpto(char* path_archivo_yamafs, char* directorio_filesystem){

}

void cpblock(char* path_archivo, int nro_bloque, int id_nodo){

}

//void md5(char* path_archivo_yamafs){

//}

void ls(char* path_directorio){

}

void info(char* path_archivo){

}



//FUNCIONES AUXILIARES//

char *yamaGetNFile(char *path){

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

char *yamaGetPathPadre(char *path){

	char** subpath = string_split(path, "/");

	char *pathPadre = string_new();
	char *subpathPadre = string_new();
	t_queue *colaPathPadre = queue_create();

	void closure(char *value){
		queue_push(colaPathPadre, value);
	}
	string_iterate_lines(subpath, closure);

	while(queue_size(colaPathPadre)>1){
		char *value = queue_pop(colaPathPadre);
		string_append_with_format(&subpathPadre,"%s/", value);
		free(pathPadre);
		pathPadre = string_substring_until(subpathPadre, string_length(subpathPadre) - 1);
	}

	free(subpathPadre);
	queue_destroy_and_destroy_elements(colaPathPadre, free);

	free(subpath);

	return pathPadre;
}

Directorio yamaCrearDirectorio(int id, char* nombre, int id_padre){

	Directorio newDir;
	newDir.id = id;
	strcpy(newDir.nombre , nombre);
	newDir.padre = id_padre;

	return newDir;
}

void asignarEspacioEnTabla(Directorio newDir){

	tabla_Directorios[newDir.id].id = newDir.id;
	strcpy(tabla_Directorios[newDir.id].nombre, newDir.nombre);
	tabla_Directorios[newDir.id].padre = newDir.padre;
}