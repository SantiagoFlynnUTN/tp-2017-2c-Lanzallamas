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
#include "EnvioArchivo.h"
#include "cliente.h"
#include "servidorMaster.h"
#include "conexionesYAMA.h"
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

void mandarTransformacionNodo(int socket_nodo, int cantidadWorkers){
	Transformacion t;
	t.transformacion = 1;
	t.cantidadWorkers = cantidadWorkers;

	if (send(socket_nodo, &t, sizeof(t), 0) ==-1)
			printf("No puedo enviar\n");
}
