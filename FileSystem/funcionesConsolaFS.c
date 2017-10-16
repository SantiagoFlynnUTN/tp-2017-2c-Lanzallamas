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
			formatFileSystem()
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
			rename(linea[1]);
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
			cpblock(linea[1], linea[2]);
			continue;
		}

		/*
		 * Funicon md5
		 */

		if (strcmp("md5", linea[0]) == 0){
			md5(linea);
			continue;
		}

		/*
		 * Funcion Ls
		 */

		if (strcmp("ls", linea[0]) == 0){
			ls(linea[1]);
			continue;
		}

		/*
		 * Funicon info
		 */

		if (strcmp("info", linea[0]) == 0){
			info(linea[1]);
			continue;
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

/************************FUNCIONES DE CONSOLA****************************/

