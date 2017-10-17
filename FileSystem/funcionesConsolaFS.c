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
#include "utilidadesFileSystem.h"

void formatFileSystem();
void rm(char ** linea);
void renameFs(char * nombreOriginal, char * nombreFinal);
void mv(char * nombreOriginal, char * nombreFinal);
void cat(char * nombre);
void mkdir(char * dir);
void cpfrom(char * archivo, char * archivoFS);
void cpto(char * archivoFS, char * archivo);
void cpblock(char * archivo, char * numeroBloque, char * nodo);
void md5(char * archivo);
void ls(char * dir);
void info(char * archivo);

void hiloConsola(){
	printf("Consola disponible par uso\n");
	while(1){

		char ** linea;
		linea = string_split(readline(">"), " ");

		/*
		 *  Funcion Exit de consola
		 */
		if (!linea || strcmp("exit", linea[0]) == 0) {
			printf("Te aburriste de la consola\n");
			break;
		}

		/*
		 * Funcion Format
		 */

		if (strcmp("format", linea[0]) == 0){
			formatFileSystem();
			continue;
		}

		/*
		 * Funcion Rm
		 */

		if (strcmp("rm", linea[0]) == 0){
			rm(linea);
			continue;
		}

		/*
		 * Funcion Rename
		 */

		if (strcmp("rename", linea[0]) == 0){
			renameFs(linea[1], linea[2]);
			continue;
		}

		/*
		 * Funcion Mv
		 */

		if (strcmp("mv", linea[0]) == 0){
			mv(linea[1], linea[2]);
			continue;
		}

		/*
		 * Funcion Cat
		 */

		if (strcmp("cat", linea[0]) == 0){
			cat(linea[1]);
			continue;
		}

		/*
		 * Funcion mkdir
		 */

		if (strcmp("mkdir", linea[0]) == 0){
			mkdir(linea[1]);
			continue;
		}

		/*
		 * Funcion CpFrom
		 */

		if (strcmp("cpfrom", linea[0]) == 0){
			cpfrom(linea[1], linea[2]);
			continue;
		}

		/*
		 * Funcion CpTo
		 */

		if (strcmp("cpto", linea[0]) == 0){
			cpto(linea[1], linea[2]);
			continue;
		}

		/*
		 * Funcion CpBlock
		 */

		if (strcmp("cpblock", linea[0]) == 0){
			cpblock(linea[1], linea[2], linea[3]);
			continue;
		}

		/*
		 * Funicon md5
		 */

		if (strcmp("md5", linea[0]) == 0){
			md5(linea[1]);
			continue;
		}

		/*
		 * Funcion Ls
		 */

		if (strcmp("ls", linea[0]) == 0){
			if(linea[1] == NULL){
				printf("Error: falta el nombre del directorio\n");
			}else{
				ls(linea[1]);
			}
			continue;
		}

		/*
		 * Funicon info
		 */

		if (strcmp("info", linea[0]) == 0){
			if(linea[1] == NULL){
				printf("Error: falta el nombre del archivo\n");
			}else{
				info(linea[1]);
			}

			continue;
		}

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

/************************FUNCIONES DE CONSOLA****************************/

void formatFileSystem(){}
void rm(char ** linea){}
void renameFs(char * nombreOriginal, char * nombreFinal){}
void mv(char * nombreOriginal, char * nombreFinal){}
void cat(char * nombre){}
void mkdir(char * dir){}
void cpfrom(char * archivo, char * archivoFS){}
void cpto(char * archivoFS, char * archivo){}
void cpblock(char * archivo, char * numeroBloque, char * nodo){}
void md5(char * archivo){}

void ls(char * dir){
	int entradaDirectorio = calcularEntradaDirectorio(dir);

	if(entradaDirectorio == -1){
		printf("El directorio no existe\n");
		return;
	}

	void * _descriptorArchivoANombre(void * archivo){
		Archivo * descriptorArchivo = (Archivo *)archivo;
		return obtenerNombreArchivo(descriptorArchivo->ruta);
	}

	t_list * nombres = list_map(listaArchivosDirectorios[entradaDirectorio], _descriptorArchivoANombre);

	void _printNombres(void* nombre) {
		printf("%s\n", (char *)nombre);
	}

	list_add_all(nombres, obtenerNombresDirectoriosHijos(entradaDirectorio));

	bool _ordenarPorNombre(void * n1, void * n2){
		char * nombre1 = (char *) n1,
		     * nombre2 = (char *) n2;

		return strcmp(nombre1, nombre2) <= 0;
	}

	list_sort(nombres, _ordenarPorNombre);

	list_iterate(nombres, _printNombres);

	list_destroy(nombres);
}

void info(char * archivo){
	Archivo * descriptorArchivo = (Archivo *) dictionary_get(archivos, archivo);

	if(descriptorArchivo == NULL){
		printf("El archivo no existe\n");
		return;
	}

	int bloques = list_size(descriptorArchivo->bloques);
	int i;

	printf("%s:\nTamaño:%ld\nBloques:%d\n", archivo, descriptorArchivo->tamanio, bloques);

	for(i = 0; i < bloques; ++i){
		Bloque * bloque = (Bloque *) list_get(descriptorArchivo->bloques, i);

		printf("Bloque %d:\t\tNodo\t\tNumero Bloque\nCopia 0:\t\t%s\t\t%d\nCopia 1:\t\t%s\t\t%d\n",
		i, bloque->copia0.nodo, bloque->copia0.numeroBloque, bloque->copia1.nodo, bloque->copia1.numeroBloque);
	}
}