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
#include "conexionesYAMA.h"
#include <protocoloComunicacion.h>
#include "transformacionMaster.h"
#include "reduccionMaster.h"
#include <sys/time.h>
#include "RGMaster.h"
#include <commons/collections/list.h>


int socket_yama;
int socket_nodo;
pthread_mutex_t mutexTransformacion = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexReduccion = PTHREAD_MUTEX_INITIALIZER;
double tiempoTotalJob = 0;
double tiempoTotalTransf = 0;
double tiempoTotalRedu = 0;
int cantTransfActual = 0;
int cantReduActual = 0;
int transformacionesOk = 0;
int reduccionesOk = 0;
int fallosTransf = 0;
int fallosRedu = 0;
int cargaMaxima = 0;
int maxT = 0;
int maxR = 0;


void printMetrics(){
	log_info(logger,"\n\n\n----------->  METRICAS  <-----------");
	printf("[Tiempo] Job: %f sec\n", tiempoTotalJob);
	printf("[Tiempo Promedio] Transformacion: %f sec\n",
			tiempoTotalTransf / transformacionesOk);
	printf("[Tiempo Promedio] Reduccion: %f sec\n",
			tiempoTotalRedu / reduccionesOk);
	printf(
			"[Cantidad] Operaciones en forma paralela:\n-> Total: %i\n-> Transformaciones: %i\n-> Reducciones: %i\n",
			 cargaMaxima, maxT, maxR);
	printf("[Cantidad] Transformaciones OK: %i\n", transformacionesOk);
	printf("[Cantidad] Reducciones OK: %i\n", reduccionesOk);
	printf("[Cantidad] Fallos en Transformacion: %i\n", fallosTransf);
	printf("[Cantidad] Fallos en Reduccion: %i\n", fallosRedu);
}

void calcularMaximos(){
	int cargaActual;
	if(cantTransfActual && cantReduActual){
		cargaActual = cantTransfActual + cantReduActual;
		if (cargaMaxima < cargaActual){
			cargaMaxima = cargaActual;
			maxT = cantTransfActual;
			maxR = cantReduActual;
		}
	}
}

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
	t_list* tiemposTransformaciones = list_create();
	mandarTransformacionNodo(socket_nodo, socket_yama, cantidadWorkersEjemplo, tiemposTransformaciones);

	reduccionGlobal(socket_yama);

	gettimeofday(&tv2, NULL);
	tiempoTotalJob = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
	         (double) (tv2.tv_sec - tv1.tv_sec);
	printf ("Tiempo de ejecución del Job = %f segundos\n",
	         tiempoTotalJob);

	printMetrics();
	return 0;
}
