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

void mkdir(char* path_dir){

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







