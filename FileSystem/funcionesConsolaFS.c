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
#include "recepcionBloques.h"
#include "md5/md5.h"
#include <sys/stat.h>
#include "envioBloques.h"

void formatFileSystem();
void rm(char ** linea);
void renameFs(char * nombreOriginal, char * nombreFinal);
void mv(char * nombreOriginal, char * nombreFinal);
void cat(char * nombre);
void mkdirConsola(char * dir);
void cpfrom(char * archivo, char * archivoFS);
void cpto(char * archivoFS, char * archivo);
void cpblock(char * archivo, char * numeroBloque, char * nodo);
void md5Consola(char * archivo);
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
			if(linea[1] == NULL){
				printf("Error: falta especificar el archivo, bloque o directorio\n");
			}else{
				rm(linea);
			}
			continue;
		}

		/*
		 * Funcion Rename
		 */

		if (strcmp("rename", linea[0]) == 0){
			if(linea[1] == NULL){
				printf("Error: falta la ruta original\n");
			}else if(linea[2] == NULL){
				printf("Error: falta el nombre final\n");
			}else{
				renameFs(linea[1], linea[2]);
			}
			continue;
		}

		/*
		 * Funcion Mv
		 */

		if (strcmp("mv", linea[0]) == 0){
			if(linea[1] == NULL){
				printf("Error: falta la ruta original\n");
			}else if(linea[2] == NULL){
				printf("Error: falta la ruta destino\n");
			}else{
				mv(linea[1], linea[2]);
			}
			continue;
		}

		/*
		 * Funcion Cat
		 */

		if (strcmp("cat", linea[0]) == 0){
			if(linea[1] == NULL){
				printf("Error: falta el nombre de archivo\n");
			}else{
				cat(linea[1]);
			}
			continue;
		}

		/*
		 * Funcion mkdir
		 */

		if (strcmp("mkdir", linea[0]) == 0){
			if(linea[1] == NULL){
				printf("Error: falta el nombre de directorio\n");
			}else{
				mkdirConsola(linea[1]);
			}
			continue;
		}

		/*
		 * Funcion CpFrom
		 */

		if (strcmp("cpfrom", linea[0]) == 0){
			if(linea[1] == NULL){
				printf("Error: falta el nombre de archivo\n");
			}else if(linea[2] == NULL) {
				printf("Error: falta el nombre de archivo en YAMA FS\n");
			}else{
				cpfrom(linea[1], linea[2]);
			}

			continue;
		}

		/*
		 * Funcion CpTo
		 */

		if (strcmp("cpto", linea[0]) == 0){
			if(linea[1] == NULL){
				printf("Error: falta el nombre de archivo en YAMA FS\n");
			}else if(linea[2] == NULL) {
				printf("Error: falta el nombre de archivo\n");
			}else{
				cpto(linea[1], linea[2]);
			}

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
			if(linea[1] == NULL){
				printf("Error: falta el nombre de archivo\n");
			}else{
				md5Consola(linea[1]);
			}
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

		printf("Error: comando desconocido\n");
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

void formatFileSystem(){
	int i;
	for(i = 1; i < 100; ++i){
		tabla_Directorios[i].id = 0;
		tabla_Directorios[i].padre = 0;
		memset(tabla_Directorios[directorio].nombre, 0, sizeof(char) * 255);
	}

}

void rm(char ** linea){
	if(linea[1][0] == '-'){
		if(linea[2] == NULL){
			printf("Error: falta especificar el archivo, bloque o directorio\n");
			return;
		}

		if(linea[1][1] == 'd'){
			int directorio = calcularEntradaDirectorio(linea[2]);

			if(directorio == -1){
				printf("No existe el archivo o directorio %s\n", linea[2]);
				return;
			}

			if(list_size(listaArchivosDirectorios[directorio]) > 0 || cantidadDirectoriosHijos(directorio) > 0){
				printf("El directorio %s no está vacío\n", linea[2]);
				return;
			}

			list_destroy(listaArchivosDirectorios[directorio]);

			tabla_Directorios[directorio].padre = 0;
			tabla_Directorios[directorio].id = 0;
			memset(tabla_Directorios[directorio].nombre, 0, sizeof(char) * 255);
		}else if(linea[1][1] == 'b'){
			if(linea[3] == NULL){
				printf("Error: falta especificar el número de bloque\n");
				return;
			}

			if(linea[4] == NULL){
				printf("Error: falta especificar el número de copia\n");
				return;
			}

			printf("BORRANDO BLOQUE\n");
		}else{
			printf("Error: opción desconocida");
		}
	}else{
		Archivo * descriptorArchivo = (Archivo *) dictionary_remove(archivos, linea[1]);

		if(descriptorArchivo == NULL){
			printf("Error: No existe el archivo %s", linea[1]);
			return;
		}

		bool criterio(void * elemento){
			Archivo * descriptor = (Archivo *) elemento;
			return strcmp(descriptor->ruta, descriptorArchivo->ruta) == 0;
		}

		list_remove_by_condition(listaArchivosDirectorios[descriptorArchivo->directorioPadre], criterio);

		destruirArchivo(descriptorArchivo);
	}
}

void renameFs(char * nombreOriginal, char * nombreFinal){
	int directorioPadre = calcularDirectorioPadre(nombreOriginal);

	if(directorioPadre == -1){
		printf("No existe la ruta %s\n", nombreOriginal);
		return;
	}

	Archivo * descriptorArchivo = (Archivo *) dictionary_remove(archivos, nombreOriginal);

	if(descriptorArchivo != NULL){
		bool criterio(void * elemento){
			Archivo * descriptor = (Archivo *) elemento;
			return strcmp(descriptor->ruta, descriptorArchivo->ruta) == 0;
		}

		list_remove_by_condition(listaArchivosDirectorios[directorioPadre], criterio);

		strcpy(descriptorArchivo->ruta, nombreFinal);

		list_add(listaArchivosDirectorios[directorioPadre], descriptorArchivo);
		dictionary_put(archivos, descriptorArchivo->ruta, descriptorArchivo);
	}else{
		int directorio = calcularEntradaDirectorio(nombreOriginal);

		if(directorio == -1){
			printf("No existe el archivo o directorio %s\n", nombreOriginal);
			return;
		}

		strcpy(tabla_Directorios[directorio].nombre, obtenerNombreArchivo(nombreFinal));

		if(list_size(listaArchivosDirectorios[directorio]) > 0){
			void actualizarRuta(void * elemento){
				Archivo * descriptor = (Archivo *) elemento;

				dictionary_remove(archivos, descriptor->ruta);

				obtenerNuevaRutaArchivo(descriptor->ruta, nombreFinal);

				dictionary_put(archivos, descriptor->ruta, descriptor);
			}

			list_iterate(listaArchivosDirectorios[directorio], actualizarRuta);
		}

		int i;

		for(i = 0; i < 100; ++i){
			if(tabla_Directorios[i].padre == directorio && strlen(tabla_Directorios[i].nombre) > 0){
				char dirHijo[255];
				strcpy(dirHijo, tabla_Directorios[i].nombre);
				obtenerNuevaRutaArchivo(dirHijo, nombreFinal);
				printf("El directorio de %s será %s\n", tabla_Directorios[i].nombre, dirHijo);

				mv(dirHijo, dirHijo);
			}
		}
	}
}

void mv(char * nombreOriginal, char * nombreFinal){
	int directorioPadreViejo = calcularDirectorioPadre(nombreOriginal);
	int directorioPadreNuevo = calcularDirectorioPadre(nombreFinal);

	if(directorioPadreViejo == -1){
		printf("No existe la ruta %s\n", nombreOriginal);
		return;
	}

	if(directorioPadreNuevo == -1){
		printf("No existe la ruta %s\n", nombreFinal);
	}

	Archivo * descriptorArchivo = (Archivo *) dictionary_remove(archivos, nombreOriginal);

	if(descriptorArchivo != NULL){
		bool criterio(void * elemento){
			Archivo * descriptor = (Archivo *) elemento;
			return strcmp(descriptor->ruta, descriptorArchivo->ruta) == 0;
		}

		list_remove_by_condition(listaArchivosDirectorios[directorioPadreViejo], criterio);

		strcpy(descriptorArchivo->ruta, nombreFinal);
		descriptorArchivo->directorioPadre = directorioPadreNuevo;

		list_add(listaArchivosDirectorios[directorioPadreNuevo], descriptorArchivo);
		dictionary_put(archivos, descriptorArchivo->ruta, descriptorArchivo);
	}else{
		int directorio = calcularEntradaDirectorio(nombreOriginal);

		if(directorio == -1){
			printf("No existe el archivo o directorio %s\n", nombreOriginal);
			return;
		}

		strcpy(tabla_Directorios[directorio].nombre, obtenerNombreArchivo(nombreFinal));
		tabla_Directorios[directorio].padre = directorioPadreNuevo;


		if(list_size(listaArchivosDirectorios[directorio]) > 0){
			void actualizarRuta(void * elemento){
				Archivo * descriptor = (Archivo *) elemento;

				dictionary_remove(archivos, descriptor->ruta);

				obtenerNuevaRutaArchivo(descriptor->ruta, nombreFinal);

				dictionary_put(archivos, descriptor->ruta, descriptor);
			}

			list_iterate(listaArchivosDirectorios[directorio], actualizarRuta);
		}

		int i;

		for(i = 0; i < 100; ++i){
			if(tabla_Directorios[i].padre == directorio && strlen(tabla_Directorios[i].nombre) > 0){
				char dirHijo[255];
				strcpy(dirHijo, tabla_Directorios[i].nombre);
				obtenerNuevaRutaArchivo(dirHijo, nombreFinal);
				printf("El directorio de %s será %s\n", tabla_Directorios[i].nombre, dirHijo);

				mv(dirHijo, dirHijo);
			}
		}
	}
}

void cat(char * nombre){
	if(obtenerArchivo(nombre, TEMPFILE) == 0){
		FILE * fd = fopen(TEMPFILE, "r");

		struct stat fileStats;

		if(fd == NULL || stat(TEMPFILE, &fileStats) != 0){ // error
			printf("No se pudo traer correctamente el archivo de los data node\n");
			return;
		}

		char contenido[fileStats.st_size];

		if(fread(contenido, fileStats.st_size * sizeof(char), 1, fd) != 1){
			printf("Error leyendo el archivo\n");
		}

		printf("%s\n", contenido);
	}
}

void mkdirConsola(char * dir){
	int entrada = calcularEntradaDirectorio(dir);

	if(entrada != -1){
		printf("Ya existe el directorio\n");
		return;
	}

	int directorioPadre = calcularDirectorioPadre(dir);

	if(directorioPadre == -1){
		printf("No existe la ruta\n");
	}

	int idDirectorio = obtenerIdDirectorio();

	if(idDirectorio == -1){
		printf("Se ha alcanzado la máxima cantidad de directorios\n");
		return;
	}

	strcpy(tabla_Directorios[idDirectorio].nombre, obtenerNombreArchivo(dir));
	tabla_Directorios[idDirectorio].padre = directorioPadre;
	tabla_Directorios[idDirectorio].id = idDirectorio;
	listaArchivosDirectorios[idDirectorio] = list_create();
}

void cpfrom(char * archivo, char * archivoFS){
	if(enviarBloques(archivo, archivoFS) != 0){
		printf("No se pudo guardar el archivo en el file system\n");
	}
}

void cpto(char * archivoFS, char * archivo){
	if(obtenerArchivo(archivoFS, archivo) != 0){
		printf("No se pudo obtener el archivo de YAMA FS\n");
	}
}

void cpblock(char * archivo, char * numeroBloque, char * nodo){








}

void md5Consola(char * archivo){
	if(obtenerArchivo(archivo, TEMPFILE) == 0){
		FILE * fd = fopen(TEMPFILE, "r");

		struct stat fileStats;

		if(fd == NULL || stat(TEMPFILE, &fileStats) != 0){ // error
			printf("No se pudo traer correctamente el archivo de los data node\n");
			return;
		}

		char contenido[fileStats.st_size];

		if(fread(contenido, fileStats.st_size * sizeof(char), 1, fd) != 1){
			printf("Error leyendo el archivo\n");
		}

		char md5Value[32];

		getMD5(contenido, fileStats.st_size, md5Value);

		printf("%s\n", md5Value);
	}
}

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
