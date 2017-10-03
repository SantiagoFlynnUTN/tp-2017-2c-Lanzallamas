/*
 * reduccionMaster.c
 *
 *  Created on: 1/10/2017
 *      Author: utnso
 */
#include <protocoloComunicacion.h>
#include "master.h"
#include <sys/types.h>
#include <sys/socket.h>

void solicitudReduccion(int socket_yama){

	int pedRed;
	pedRed = PEDIDOREDUCCION;
	if (send(socket_yama, &pedRed, sizeof(int), 0) ==-1)
						printf("No puedo enviar\n");
	operacionReduccion op;
	recv(socket_yama, &op, sizeof(op), 0);

	rutaArchivo rutas[op.cantidadTemporales];

	int i = 0;
	while (op.cantidadTemporales--) {
		recv(socket_yama, &rutas[op.cantidadTemporales], sizeof(rutaArchivo),
				0);
		printf("ruta: %s\n", &rutas[op.cantidadTemporales]);
		i++;
	}

	int cantidadNodosEjemplo = 3;

	mandarReduccionNodo(op, rutas, i, cantidadNodosEjemplo);
}

void iniciarConexionAWorker(int *sockfd, operacionReduccion op){

	struct sockaddr_in their_addr; // información de la dirección de destino


	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
	their_addr.sin_port = op.puerto;  // short, Ordenación de bytes de la red
	their_addr.sin_addr.s_addr = inet_addr(op.ip);
	memset(&(their_addr.sin_zero), 0, 8);  // poner a cero el resto de la estructura

	if (connect(*sockfd, (struct sockaddr *)&their_addr,
										  sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}

void mandarSolicitudReduccion(operacionReduccion* op){
	int socketNodo;
	iniciarConexionAWorker(&socketNodo, *op);

	reduccionWorker mensaje;
	mensaje.tipoMensaje = 2;
	mensaje.cantidadTemporales = op->cantidadTemporales;
	strcpy(mensaje.archivoReducido, op->archivoReducido);

	if (send(socketNodo, &mensaje, sizeof(mensaje), 0) ==-1)
					printf("No puedo enviar\n");



	int a, numbytes;
	numbytes = recv(socketNodo, &a, sizeof(int), 0);
	if (a == 4){
		printf("worker %d finalizó reduccion\n", socketNodo);
		pthread_exit(NULL);
	}
}

void mandarReduccionNodo(operacionReduccion op, rutaArchivo* rutas, int cantidadRutas, int cantNodos){
	pthread_t tid[cantNodos];
	int rc[cantNodos];
	int i = 0;
	while(cantNodos--){
		rc[cantNodos] = pthread_create(&tid[cantNodos], NULL, mandarSolicitudReduccion, &op);
		if(rc[cantNodos]) printf("no pudo crear el hilo %d\n", i);
		i++;
	}

	while(i--){
		pthread_join(tid[i], NULL);
	}
	printf("Terminaron las transformaciones del nodo %d\n", socket_nodo);
}
