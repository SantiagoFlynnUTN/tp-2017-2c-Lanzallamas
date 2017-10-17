/*
    ** client.c -- Ejemplo de cliente de sockets de flujo
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
#include "cliente.h"
#include "servidorMaster.h"
#include "conexionesYAMA.h"
#include <protocoloComunicacion.h>
#include "transformacionMaster.h"
#include "reduccionMaster.h"


int socket_yama;
int socket_nodo;

int main(int argc, char *argv[]){

	if(argc < 5){
		printf("faltan parametros\n");
		exit(-1);
	}

	strcpy(argv[3], "algo.txt");

	inicializarMaster();
	printf("Conectando a YAMA...\n");
	iniciarConexionAYAMA(&socket_yama);
	printf("Enviando solicitud de Job\n");
	solicitudJob(socket_yama, argv[3]);

	int cantidadWorkersEjemplo;
	cantidadWorkersEjemplo = respuestaSolicitud(socket_yama);


	mandarTransformacionNodo(socket_nodo, socket_yama, cantidadWorkersEjemplo);

	solicitudReduccion(socket_yama);

	return 0;
}
