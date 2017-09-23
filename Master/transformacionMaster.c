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

void _handshake(int socket){
	Handshake h;
	recv(socket, &h, sizeof(h), 0);
	printf("Se conectó un worker %d:%d\n", h.parentPid, h.childPid);

	int a;
	a = recv(socket, &a, sizeof(int), 0);
	if (a <= 0){
		printf("worker %d:%d finalizó transformación\n", h.parentPid, h.childPid);
		pthread_exit(NULL);
	}
}

void _manejarDatos(int buf, int socket){

	switch(buf){
	case 1://HANDSHAKE
		_handshake(socket);
		//desligar socket al hilo principal.
		break;
	case 4:
		printf("llegue");
		break;
	}
}

void _manejarCliente(int* newfd){

	int numbytes;
	int buf;

	buf = 0;
	numbytes = recv(*newfd, &buf, sizeof(int), 0); //leo el primer byte. Me dirá el tipo de paquete. (es un int)

	comprobarConexion(numbytes, *newfd); //Me fijo si lo que recibí esta ok.

	_manejarDatos(buf, *newfd); //Si llegamos hasta acá manejamos los datos que recibimos.
}

void atenderConexion(int socket){
	int rc;
	pthread_t tid;

	rc = pthread_create(&tid, NULL, _manejarCliente, &socket);
		if(rc) printf("no pudo crear el hilo");
}

int respuestaSolicitud(int socket_yama) {

	int cantidadWorkers;
	int nbytesReceived = 0;

	if ((nbytesReceived = recv(socket_yama, &cantidadWorkers, sizeof(int), 0))
			<= 0)				//recibo y compruebo q recibí correctamente
		printf("No puedo recibir información, o el servidor colgó\n");

	return cantidadWorkers;
	/*else {
		int i;
		for(i=0; i<cantidadWorkers; i++) {
			recv(socket_yama, tablaTransformacion[i], sizeof(workerTransformacion), 0);
		}
	}*/
}

void iniciarConexionAWorker(int *sockfd, workerTransformacion t){

	struct sockaddr_in their_addr; // información de la dirección de destino


	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
	their_addr.sin_port = htons(PORTNODO);  // short, Ordenación de bytes de la red
	their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/*their_addr.sin_port = t.puertoWorker;  // short, Ordenación de bytes de la red
	their_addr.sin_addr.s_addr = inet_addr(t.ipWorker);*/
	memset(&(their_addr.sin_zero), 0, 8);  // poner a cero el resto de la estructura

	if (connect(*sockfd, (struct sockaddr *)&their_addr,
										  sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}

void mandarSolicitudTransformacion(workerTransformacion* t){
	int socketWorker;
	iniciarConexionAWorker(&socketWorker, *t);

	mensajeTransformacion mensaje;
	mensaje.tipoMensaje = 1;
	mensaje.cantidadBytes = t->bytesOcupados;
	mensaje.bloque = t->numBloque;
	strcpy(mensaje.nombreTemp, t->rutaArchivo);

	if (send(socketWorker, &mensaje, sizeof(mensaje), 0) ==-1)
					printf("No puedo enviar\n");

	enviarArchivo(socketWorker, "prueba.sh");

	printf("%d vces\n", socketWorker);
	int a, numbytes;
	numbytes = recv(socketWorker, &a, sizeof(int), 0);
	if (a == 4){
		printf("worker %d finalizó transformación\n", socketWorker);
		pthread_exit(NULL);
	}
}


void mandarTransformacionNodo(int socket_nodo, int cantidadWorkers){
	workerTransformacion t[cantidadWorkers];
	pthread_t tid[cantidadWorkers];
	int rc[cantidadWorkers];

	while(cantidadWorkers--){
		pthread_create(&tid[cantidadWorkers], NULL, mandarSolicitudTransformacion, &t[cantidadWorkers]);

	}
}
