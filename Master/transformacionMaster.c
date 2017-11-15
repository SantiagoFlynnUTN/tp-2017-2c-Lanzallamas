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
#include "conexionesYAMA.h"
#include "transformacionMaster.h"
#include <protocoloComunicacion.h>
#include <sockets.h>
#include "reduccionMaster.h"
#include <commons/collections/list.h>
#include <sys/time.h>

int respuestaSolicitud() {

	int cantidadWorkers;

	zrecv(socket_yama, &jobId, sizeof(jobId), 0);
	log_info(logger, "Job Id: %d", jobId);

	zrecv(socket_yama, &cantidadWorkers, sizeof(int), 0);

	return cantidadWorkers;
}

int conexionTransfWorker(int *sockfd, workerTransformacion t){

	struct sockaddr_in their_addr; // información de la dirección de destino


	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return(1);
	}

	log_info(logger, "ip: %s\nport: %d\n", t.ipWorker, t.puertoWorker);

	their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
	their_addr.sin_port = t.puertoWorker;  // short, Ordenación de bytes de la red
	their_addr.sin_addr.s_addr = inet_addr(t.ipWorker);
	memset(&(their_addr.sin_zero), 0, 8);  // poner a cero el resto de la estructura

	if (connect(*sockfd, (struct sockaddr *)&their_addr,
				sizeof(struct sockaddr)) == -1) {
		perror("connect");
		return(1);
	}
	return 0;
}

void mandarSolicitudTransformacion(workerTransformacion* t) {
	int socketWorker;
	int tipoMensaje;
	struct timeval tv1, tv2;

	pthread_mutex_lock(&mutexTransformacion);
	cantTransfActual++;
	calcularMaximos();
	pthread_mutex_unlock(&mutexTransformacion);

	gettimeofday(&tv1, NULL);
	if (conexionTransfWorker(&socketWorker, *t)) {
		log_error(logger, "No se pudo conectar al worker. Replanificar.\n");
	}

	mensajeTransformacion mensaje;
	tipoMensaje = 1;
	mensaje.cantidadBytes = t->bytesOcupados;
	mensaje.bloque = t->numBloque;
	strcpy(mensaje.nombreTemp, t->rutaArchivo);

	zsend(socketWorker, &tipoMensaje, sizeof(int), 0);
	zsend(socketWorker, &mensaje, sizeof(mensaje), 0);

	enviarArchivo(socketWorker, transformador);

	int status, bytes;
	bytes = recv(socketWorker, &status, sizeof(int), 0);
	if (bytes == -1 || status != 0) {
		int tipomensaje = FALLOTRANSFORMACION;
		transfError mensajeError;
		strcpy(mensajeError.nombreNodo, t->nombreNodo);
		strcpy(mensajeError.nombreTemp, t->rutaArchivo);
		mensajeError.numBloque = t->numBloque;
		mensajeError.bytes = t->bytesOcupados;
		log_error(logger, "fallo la transformacion en el nodo %s\n",
				t->nombreNodo);
		pthread_mutex_lock(&mutexTransformacion);
		cantTransfActual--;
		fallosTransf++;
		zsend(socket_yama, &tipomensaje, sizeof(tipomensaje), 0);
		zsend(socket_yama, &mensajeError, sizeof(mensajeError), 0);
		pthread_mutex_unlock(&mutexTransformacion);

	} else {
		int mensajeOK = TRANSFORMACIONOK;

		gettimeofday(&tv2, NULL);
		log_info(logger, "worker %d finalizó transformación\n", socketWorker);

		double tiempoTotal = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000
				+ (double) (tv2.tv_sec - tv1.tv_sec);
		printf("Tiempo de ejecución del Job = %f segundos\n", tiempoTotal);

		pthread_mutex_lock(&mutexTransformacion);
		tiempoTotalTransf += tiempoTotal;
		transformacionesOk++;
		cantTransfActual--;
		zsend(socket_yama, &mensajeOK, sizeof(int), 0);
		zsend(socket_yama, &jobId, sizeof(jobId), 0);
		zsend(socket_yama, mensaje.nombreTemp, sizeof(char) * 255, 0);
		pthread_mutex_unlock(&mutexTransformacion);
	}

	pthread_exit(NULL);
}

void mandarReplanificado(){
	pthread_t tid;
	int rc;
	workerTransformacion t;
	zrecv(socket_yama, t.nombreNodo, sizeof(char) * 100, 0);
	zrecv(socket_yama, t.ipWorker, sizeof(char) * 20, 0);
	zrecv(socket_yama, &t.puertoWorker, sizeof(t.puertoWorker), 0);
	zrecv(socket_yama, &t.numBloque, sizeof(t.numBloque), 0);
	zrecv(socket_yama, &t.bytesOcupados, sizeof(t.bytesOcupados), 0);
	zrecv(socket_yama, t.rutaArchivo, 255 * sizeof(char), 0);
	rc = pthread_create(&tid, NULL, (void*) mandarSolicitudTransformacion, &t);

	if (rc)
		log_error(logger, "no pudo crear el hilo %d\n");
}

void mandarTransformacionNodo() {
	int cantidadWorkers = respuestaSolicitud();
	workerTransformacion t[cantidadWorkers];
	pthread_t tid[cantidadWorkers];
	int rc[cantidadWorkers];
	int i = 0;

	while (cantidadWorkers--) {
		zrecv(socket_yama, t[cantidadWorkers].nombreNodo, sizeof(char) * 100,
				0);
		zrecv(socket_yama, t[cantidadWorkers].ipWorker, sizeof(char) * 20, 0);
		zrecv(socket_yama, &t[cantidadWorkers].puertoWorker,
				sizeof(t[cantidadWorkers].puertoWorker), 0);
		zrecv(socket_yama, &t[cantidadWorkers].numBloque,
				sizeof(t[cantidadWorkers].numBloque), 0);
		zrecv(socket_yama, &t[cantidadWorkers].bytesOcupados,
				sizeof(t[cantidadWorkers].bytesOcupados), 0);
		zrecv(socket_yama, t[cantidadWorkers].rutaArchivo, 255 * sizeof(char),
				0);

		rc[cantidadWorkers] = pthread_create(&tid[cantidadWorkers], NULL,
				(void*) mandarSolicitudTransformacion, &t[cantidadWorkers]);

		if (rc[cantidadWorkers])
			log_error(logger, "no pudo crear el hilo %d\n", i);
		i++;
	}

	int operacion = 0;
	while (operacion != SOLICITUDREDUCCIONGLOBAL) {
		zrecv(socket_yama, &operacion, sizeof(operacion), 0);
		switch (operacion) {
		case REPLANIFICACION:
			mandarReplanificado();
			break;
		case SOLICITUDREDUCCIONLOCAL:
			reduccionLocal(socket_yama);
			break;
		case FALLOTRANSFORMACION:
			log_info(logger,
					"[ABORTADO] YAMA no pudo replanificar transformacion.\n");
			exit(1);
		case FALLOREDLOCAL:
			log_info(logger,
					"[ABORTADO] YAMA no puede replanificar una reduccion.\n");
			exit(1);
		}
	}

	log_info(logger, "Terminaron las transformaciones\n");
}
