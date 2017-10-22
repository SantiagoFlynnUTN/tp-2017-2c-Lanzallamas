/*
 * reduccionMaster.c



 *
 *  Created on: 1/10/2017
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

void conexionReduccionWorker(int *sockfd, operacionReduccion op) {

	struct sockaddr_in their_addr; // información de la dirección de destino

	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
	their_addr.sin_port = op.puerto;  // short, Ordenación de bytes de la red
	their_addr.sin_addr.s_addr = inet_addr(op.ip);
	memset(&(their_addr.sin_zero), 0, 8); // poner a cero el resto de la estructura

	if (connect(*sockfd, (struct sockaddr *) &their_addr,
			sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}

void mandarSolicitudReduccion(operacionReduccion* op) {
	int socketNodo;
	int tipoMensaje;
	conexionReduccionWorker(&socketNodo, *op);

	reduccionWorker mensaje;
	tipoMensaje = PEDIDOREDUCCION;

	mensaje.cantidadTemporales = op->cantidadTemporales;
	strcpy(mensaje.archivoReducido, op->archivoReducido);

	zsend(socketNodo, &tipoMensaje, sizeof(int), 0);
	zsend(socketNodo, &mensaje, sizeof(reduccionWorker), 0);

	int a, bytes, fallo;
	bytes= recv(socketNodo, &a, sizeof(int), 0);
	if(bytes == -1){
		fallo = FALLOREDLOCAL;
		printf("Fallo reduccion en nodo %s\n", op->nombreNodo);
		zsend(YAMAsock, &fallo, sizeof(int), 0);
		//zsend(YAMAsock, op->nombreNodo, sizeof(char)*100, 0);
	}
	if (a == 4) {
		printf("worker %d finalizó reduccion\n", socketNodo);
		pthread_exit(NULL);
	}
}

void mandarReduccionNodo(operacionReduccion op, rutaArchivo* rutas,
		int cantidadRutas, int cantNodos) {
	pthread_t tid[cantNodos];
	int rc[cantNodos];
	int i = 0;
	while (cantNodos--) {
		rc[cantNodos] = pthread_create(&tid[cantNodos], NULL,
				(void*)mandarSolicitudReduccion, &op);
		if (rc[cantNodos])
			printf("no pudo crear el hilo %d\n", i);
		i++;
	}

	while (i--) {
		pthread_join(tid[i], NULL);
		printf("Terminaron las reducciones\n");
	}
}

void reduccionLocal(int socket_yama) {

	YAMAsock = socket_yama; //como dije, no me puteen, je.
	int pedRed;
	pedRed = PEDIDOREDUCCION;
	zsend(socket_yama, &pedRed, sizeof(int), 0);
	operacionReduccion op;
	recv(socket_yama, &op, sizeof(op), 0);

	rutaArchivo rutas[op.cantidadTemporales];

	int i = 0;
	while (op.cantidadTemporales--) {
		zrecv(socket_yama, &rutas[op.cantidadTemporales], sizeof(rutaArchivo),
				0);
		printf("ruta: %s\n", (char*)&rutas[op.cantidadTemporales]);
		i++;
	}

	int cantidadNodosEjemplo = 3;

	mandarReduccionNodo(op, rutas, i, cantidadNodosEjemplo);
}




