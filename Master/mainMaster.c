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
#include <sys/time.h>
#include "RGMaster.h"


int socket_yama;
int socket_nodo;

int main(int argc, char *argv[]){

	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);

	if(argc < 5){
		printf("faltan parametros\n");
		exit(-1);
	}

	inicializarMaster();
	log_info(logger, "Conectando a YAMA...\n");
	iniciarConexionAYAMA(&socket_yama);
	log_info(logger, "Enviando solicitud de Job\n");
	solicitudJob(socket_yama, argv[3]);

	int cantidadWorkersEjemplo;
	cantidadWorkersEjemplo = respuestaSolicitud(socket_yama);

	mandarTransformacionNodo(socket_nodo, socket_yama, cantidadWorkersEjemplo);

	reduccionLocal(socket_yama);

	reduccionGlobal(socket_yama);

	gettimeofday(&tv2, NULL);
	printf ("Tiempo de ejecución del Job = %f segundos\n",
	         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
	         (double) (tv2.tv_sec - tv1.tv_sec));
	return 0;
}
