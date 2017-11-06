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


void mandarReduccionNodo(OperacionReduccion op, rutaArchivo* rutas, int cantidadRutas, int cantNodos);

void solicitudReduccion(int socket_yama) {

	YAMAsock = socket_yama; //como dije, no me puteen, je.
	int pedRed;
	pedRed = PEDIDOREDUCCION;
	zsend(socket_yama, &pedRed, sizeof(int), 0);
	OperacionReduccion op;
	recv(socket_yama, &op, sizeof(op), 0);

	rutaArchivo rutas[op.cantidadTemporales];

	int i = 0;
	while (op.cantidadTemporales--) {
		printf("llegue\n");
		zrecv(socket_yama, rutas[op.cantidadTemporales].ruta, sizeof(rutaArchivo),
			  0);
		printf("ruta: %s\n", rutas[op.cantidadTemporales].ruta);
		i++;
	}

	int cantidadNodosEjemplo = 3;

	mandarReduccionNodo(op, rutas, i, cantidadNodosEjemplo);
}

void conexionReduccionWorker(int *sockfd, OperacionReduccion op) {

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

void mandarSolicitudReduccion(OperacionReduccion* op) {
	int socketNodo;
	int tipoMensaje;
	conexionReduccionWorker(&socketNodo, *op);

	reduccionWorker mensaje;
	tipoMensaje = PEDIDOREDUCCION;

	mensaje.cantidadTemporales = op->cantidadTemporales;
	strcpy(mensaje.archivoReducido, op->archivoReducido);

	zsend(socketNodo, &tipoMensaje, sizeof(int), 0);
	zsend(socketNodo, &mensaje, sizeof(reduccionWorker), 0);

	int status, bytes;
	bytes= recv(socketNodo, &status, sizeof(int), 0);
	if(bytes == -1 || status != 0){
		int mensajeError = FALLOREDLOCAL;
		log_error(logger, "Fallo reduccion en nodo %s\n", op->nombreNodo);
		zsend(YAMAsock, &mensajeError, sizeof(int), 0);
	}else{
		log_info(logger, "worker %d finalizó reduccion\n", socketNodo);
	}
	pthread_exit(NULL);
}

void mandarReduccionNodo(OperacionReduccion op, rutaArchivo* rutas, int cantidadRutas, int cantNodos) {
	pthread_t tid[cantNodos];
	int rc[cantNodos];
	int i = 0;
	while (cantNodos--) {
		rc[cantNodos] = pthread_create(&tid[cantNodos], NULL,
									   (void*)mandarSolicitudReduccion, &op);
		if (rc[cantNodos])
			log_error(logger, "no pudo crear el hilo %d\n", i);
		i++;
	}

	while (i--) {
		pthread_join(tid[i], NULL);
		log_info(logger, "Terminaron las reducciones\n");
	}
}

void reduccionLocal(int socket_yama) {
	OperacionReduccion op;

	zrecv(socket_yama, op.nombreNodo, sizeof(char) * 100, 0);
	zrecv(socket_yama, op.ip, sizeof(char) * 20, 0);
	zrecv(socket_yama, &op.puerto, sizeof(op.puerto), 0);
	zrecv(socket_yama, &op.cantidadTemporales, sizeof(op.cantidadTemporales), 0);

	int i;

	printf("Reduccion a Realizar:\nNODO: %s\nIP: %s\nPUERTO: %d\n", op.nombreNodo, op.ip, op.puerto);

	for(i = 0; i < op.cantidadTemporales; ++i){
		zrecv(socket_yama, op.temporales[i], sizeof(char) * 255, 0);

		printf("TEMPORAL %d: %s\n", i+1, op.temporales[i]);
	}

	zrecv(socket_yama, op.archivoReducido, sizeof(char) *  255, 0);

	printf("REDUCIDO: %s\n", op.archivoReducido);
}