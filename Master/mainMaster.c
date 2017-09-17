/*
    ** client.c -- Ejemplo de cliente de sockets de flujo
    */

#include "mainMaster.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "chat.h"
#include "EnvioArchivo.h"
#include "cliente.h"
#include "conexionesYAMA.h"

#define PORT 9034 // puerto al que vamos a conectar

#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez

#define OK 1
#define ESTRUCTURA 2
#define ARCHIVO 3

int sockfd;
int cantidadWorkers;
workerTransformacion* tablaTransformacion[];

void manejarDatos(int buf, int socket){
	switch(buf){
	case OK:
		printf("socket %i dice OK\n", socket);
		break;
	case ESTRUCTURA:
		//por ahora manejamos solo mensajes
		leerMensaje(socket);
		break;
	case ARCHIVO:
		printf("crear funcion para deserializar archivo\n");
		break;
	}
}

void respuestaSolicitud(){

	int nbytesReceived = 0;
	while(nbytesReceived == 0){		//Tanto FS, como memoria, y cpu saben solo escuchar mensajes por el momento, asi q quedamos en bucle hasta la muerte.
		cantidadWorkers = 0;

		if((nbytesReceived = recv(sockfd, &cantidadWorkers, sizeof(int), 0)) <= 0)				//recibo y compruebo q recibí correctamente
			printf("No puedo recibir información, o el servidor colgó\n");

		else{
			int i;
			for(i=0; i<cantidadWorkers; i++){
				recv(sockfd, tablaTransformacion[i], sizeof(workerTransformacion), 0);
			}
		}
	}
}

void iniciarTransfWorker(){

}

void crearHilosTransformacion(){
	int i;
	int rc[cantidadWorkers];
	pthread_t tid[cantidadWorkers];
	for(i=0; i<cantidadWorkers; i++){
		rc[i] = pthread_create(&tid[i], NULL, iniciarTransfWorker, NULL);
			if(rc[i]) printf("no pudo crear el hilo %d", i);
	}
}

int main(int argc, char *argv[]){
	inicializarMaster();
	iniciarConexionAYAMA(&sockfd, PORT);
	solicitudJob(sockfd, "algo.txt");
	respuestaSolicitud();
	crearHilosTransformacion();
//	enviarArchivo(sockfd, "prueba.sh");

	/* asociarAYAMA();
	 * ingresarComando();
	 * esperarIndicaciones();
	 * thread por worker: transformacion();
	 * thread por nodo: reduccion();
	 *
	 *
	 */
/*
	for(;;);

	close(sockfd);*/
	return 0;
}
