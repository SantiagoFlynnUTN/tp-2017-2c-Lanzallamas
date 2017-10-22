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

	return cantidadWorkers;
}

void conexionTransfWorker(int *sockfd, workerTransformacion t){

	struct sockaddr_in their_addr; // información de la dirección de destino


	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	log_info(logger, "ip: %s\nport: %d\n", t.ipWorker, t.puertoWorker);

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
	conexionTransfWorker(&socketWorker, *t);

	mensajeTransformacion mensaje;
	mensaje.tipoMensaje = 1;
	mensaje.cantidadBytes = t->bytesOcupados;
	mensaje.bloque = t->numBloque;
	strcpy(mensaje.nombreTemp, t->rutaArchivo);

	zsend(socketWorker, &mensaje, sizeof(mensaje), 0);

	enviarArchivo(socketWorker, "prueba.sh");

	int status, bytes;
	bytes = recv(socketWorker, &status, sizeof(int), 0);
	if(bytes == -1 || status != 0){
		int mensajeError = FALLOTRANSFORMACION;
		log_error(logger, "fallo la transformacion en el nodo %s\n", t->nombreNodo);
		zsend(YAMAsock, &mensajeError, sizeof(int), 0);
	}else{
		log_info(logger, "worker %d finalizó transformación\n", socketWorker);
	}

	pthread_exit(NULL);
}

void mandarTransformacionNodo(int socket_nodo, int socket_yama,
							  int cantidadWorkers) {
	workerTransformacion t[cantidadWorkers];
	pthread_t tid[cantidadWorkers];
	int rc[cantidadWorkers];
	int i = 0;
	YAMAsock = socket_yama; //no me puteen por esto, tengo mucha paja.
	while (cantidadWorkers--) {
		zrecv(socket_yama, t[cantidadWorkers].nombreNodo, sizeof(char)*100, 0);
		zrecv(socket_yama, t[cantidadWorkers].ipWorker, sizeof(char)*20, 0);
		zrecv(socket_yama, &t[cantidadWorkers].puertoWorker, sizeof(t[cantidadWorkers].puertoWorker), 0);
		zrecv(socket_yama, &t[cantidadWorkers].numBloque, sizeof(t[cantidadWorkers].numBloque), 0);
		zrecv(socket_yama, &t[cantidadWorkers].bytesOcupados, sizeof(t[cantidadWorkers].bytesOcupados), 0);
		zrecv(socket_yama, t[cantidadWorkers].rutaArchivo, 255 * sizeof(char), 0);

		log_info(logger, "nom: %s\nbloq: %d\nbytes: %d\nip: %s\nport: %d\nruta: %s\n",
				 t[cantidadWorkers].nombreNodo, t[cantidadWorkers].numBloque,
				 t[cantidadWorkers].bytesOcupados, t[cantidadWorkers].ipWorker,
				 t[cantidadWorkers].puertoWorker,
				 t[cantidadWorkers].rutaArchivo);
		rc[cantidadWorkers] = pthread_create(&tid[cantidadWorkers], NULL,
											 mandarSolicitudTransformacion, &t[cantidadWorkers]);
		if (rc[cantidadWorkers])
			log_error(logger, "no pudo crear el hilo %d\n", i);
		i++;
	}

	while (i--) {
		pthread_join(tid[i], NULL);
	}
	log_info(logger, "Terminaron las transformaciones\n");
}
