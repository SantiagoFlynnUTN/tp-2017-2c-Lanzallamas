/*
 * transformacionMaster.c
 *
 *  Created on: 18/9/2017
 *      Author: utnso
 */

#include "mainMaster.h"
#include "master.h"
#include <stdio.h>
#include <stdlib.h>
#include <archivos.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "chat.h"
#include "enviarArchivo.h"
#include "cliente.h"
#include "servidorMaster.h"
#include "conexionesYAMA.h"
#include "transformacionMaster.h"
#include <protocoloComunicacion.h>
#include <sockets.h>

int YAMAsock;

int respuestaSolicitud(int socket_yama) {

	int cantidadWorkers;

	zrecv(socket_yama, &cantidadWorkers, sizeof(int), 0);

	//printf("cant: %d\n", cantidadWorkers);

	return cantidadWorkers;
}

void conexionTransfWorker(int *sockfd, workerTransformacion t){

	struct sockaddr_in their_addr; // información de la dirección de destino


	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	printf("ip: %s\nport: %d\n", t.ipWorker, t.puertoWorker);

	their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
	//their_addr.sin_port = t.puertoWorker;  // short, Ordenación de bytes de la red
	their_addr.sin_port = htons(PORTNODO);  // short, Ordenación de bytes de la red
	//their_addr.sin_addr.s_addr = inet_addr(t.ipWorker);
	their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(their_addr.sin_zero), 0, 8);  // poner a cero el resto de la estructura

	if (connect(*sockfd, (struct sockaddr *)&their_addr,
										  sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}

void mandarSolicitudTransformacion(workerTransformacion* t){
	int socketWorker;
	int tipoMensaje;
	conexionTransfWorker(&socketWorker, *t);

	mensajeTransformacion mensaje;
	mensaje.tipoMensaje = 1;
	mensaje.cantidadBytes = t->bytesOcupados;
	mensaje.bloque = t->numBloque;
	strcpy(mensaje.nombreTemp, t->rutaArchivo);

	zsend(socketWorker, &tipoMensaje, sizeof(int), 0);
	zsend(socketWorker, &mensaje, sizeof(mensaje), 0);

	enviarArchivo(socketWorker, "prueba.sh");

	int a, bytes;
	bytes = recv(socketWorker, &a, sizeof(int), 0);
	if(bytes == -1){
		printf("fallo la transformacion en el nodo %s\n", t->nombreNodo);
		zsend(YAMAsock, FALLOTRANSFORMACION, sizeof(int), 0);
	}

	if (a == 4){
		printf("worker %d finalizó transformación\n", socketWorker);
		pthread_exit(NULL);
	}
}


void mandarTransformacionNodo(int socket_nodo, int socket_yama,
		int cantidadWorkers) {
	workerTransformacion t[cantidadWorkers];
	pthread_t tid[cantidadWorkers];
	int rc[cantidadWorkers];
	int i = 0;
	YAMAsock = socket_yama; //no me puteen por esto, tengo mucha paja.
	while (cantidadWorkers--) {

		recv(socket_yama,t + cantidadWorkers, sizeof(workerTransformacion), 0);

		printf("nom: %s\nbloq: %d\nbytes: %d\nip: %s\nport: %d\nruta: %s\n",
				t[cantidadWorkers].nombreNodo, t[cantidadWorkers].numBloque,
				t[cantidadWorkers].bytesOcupados, t[cantidadWorkers].ipWorker,
				t[cantidadWorkers].puertoWorker,
				t[cantidadWorkers].rutaArchivo);
		rc[cantidadWorkers] = pthread_create(&tid[cantidadWorkers], NULL,
				mandarSolicitudTransformacion, &t[cantidadWorkers]);
		if (rc[cantidadWorkers])
			printf("no pudo crear el hilo %d\n", i);
		i++;
	}

	while (i--) {
		pthread_join(tid[i], NULL);
	}
	printf("Terminaron las transformaciones\n");
}
