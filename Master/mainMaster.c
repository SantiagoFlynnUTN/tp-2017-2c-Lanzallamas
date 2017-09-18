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
#include "EnvioArchivo.h"
#include "cliente.h"
#include "servidorMaster.h"
#include "conexionesYAMA.h"
#include <protocoloComunicacion.h>
#include "transformacionMaster.h"


int socket_yama;
int socket_nodo;

workerTransformacion* tablaTransformacion[];

int main(int argc, char *argv[]){

	inicializarMaster();
	iniciarConexionAYAMA(&socket_yama);
	solicitudJob(socket_yama, "algo.txt");

	int cantidadWorkersEjemplo;
	cantidadWorkersEjemplo = respuestaSolicitud(socket_yama);

	iniciarConexionANodo(&socket_nodo);
	mandarTransformacionNodo(socket_nodo, cantidadWorkersEjemplo);

	inicializarServer();

	//enviarArchivo(sockfd, "prueba.sh");




	/*
	 * (no dar bola a esto)
	 * asociarAYAMA();
	 * ingresarComando();
	 * esperarIndicaciones();
	 * thread por worker: transformacion();
	 * thread por nodo: reduccion();
	 *
	 */

	for(;;);
/*
	close(sockfd);*/
	return 0;
}
