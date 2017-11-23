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
#include <sockets.h>


int socket_yama;
int socket_nodo;
pthread_mutex_t yamaMensajes = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexTransformacion = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexReduccion = PTHREAD_MUTEX_INITIALIZER;
double tiempoTotalJob = 0;
double tiempoTotalTransf = 0;
double tiempoTotalRedu = 0;
double tiempoTotalGlo = 0;
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
	printf("[Tiempo Promedio] Reducciones: %f sec\n",
			tiempoTotalRedu / reduccionesOk);
	printf("[Tiempo Total] Global: %f sec\n", tiempoTotalGlo);
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

void _almacenamiento(int socket_yama, char * archivoFinal);
void _conexionAlmacenamientoWorker(int *sockfd, OperacionReduccionGlobal op);

int main(int argc, char *argv[]){

	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);

	if(argc < 5){
		printf("faltan parametros\n");
		exit(-1);
	}

	transformador = argv[1];
	reductor = argv[2];
	pathArchivoFS = argv[4];

	inicializarMaster();
	log_info(logger, "Conectando a YAMA...\n");
	iniciarConexionAYAMA(&socket_yama);
	log_info(logger, "Enviando solicitud de Job\n");
	solicitudJob(socket_yama, argv[3]);

	mandarTransformacionNodo();

	reduccionGlobal(socket_yama);

	_almacenamiento(socket_yama, argv[4]);

	gettimeofday(&tv2, NULL);
	tiempoTotalJob = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
	         (double) (tv2.tv_sec - tv1.tv_sec);
	printf ("Tiempo de ejecución del Job = %f segundos\n",
	         tiempoTotalJob);

	printMetrics();
	return 0;
}

void _almacenamiento(int socket_yama, char * archivoFinal){
	int almacenamiento;
	zrecv(socket_yama, &almacenamiento, sizeof(almacenamiento), 0);

	if(almacenamiento != SOLICITUDALMACENAMIENTO){
		return;
	}

	OperacionReduccionGlobal op;

	zrecv(socket_yama, op.nombreNodo, sizeof(char) * 100,  0);
	zrecv(socket_yama, op.ip, sizeof(char) * 20, 0);
	zrecv(socket_yama, &op.puerto, sizeof(op.puerto), 0);
	zrecv(socket_yama, op.archivoReducido, sizeof(char) * 255, 0);

	log_info(logger, "ALMACENANDO: %s  %s:%d\t%s", op.nombreNodo, op.ip, op.puerto, op.archivoReducido);

	int socketWorker;
	_conexionAlmacenamientoWorker(&socketWorker, op);

	int mensaje = ALMACENAMIENTO;
	zsend(socketWorker, &mensaje, sizeof(mensaje), 0);
	zsend(socketWorker, op.archivoReducido, sizeof(char) * 255, 0);
	zsend(socketWorker, archivoFinal, sizeof(char) * 255, 0);

	int status;
	if(recv(socketWorker, &status, sizeof(int), MSG_WAITALL) == -1 || status != 0){
		int mensajeError = ERRORALMACENAMIENTO;
		log_error(logger, "Fallo el almacenamiento en nodo %s\n", op.nombreNodo);
		zsend(socket_yama, &mensajeError, sizeof(int), 0);
	}else{
		int mensajeOK = ALMACENAMIENTOOK;
		log_info(logger, "worker %d finalizó el almacenamiento\n", socketWorker);
		zsend(socket_yama, &mensajeOK, sizeof(int), 0);
	}

	zsend(socket_yama, &jobId, sizeof(jobId), 0);
}


void _conexionAlmacenamientoWorker(int *sockfd, OperacionReduccionGlobal op) {

	struct sockaddr_in their_addr; // información de la dirección de destino

	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
	their_addr.sin_port = op.puerto;
	//their_addr.sin_addr.s_addr = inet_addr(t.ipWorker);
	their_addr.sin_addr.s_addr = inet_addr(op.ip);
	memset(&(their_addr.sin_zero), 0, 8); // poner a cero el resto de la estructura

	if (connect(*sockfd, (struct sockaddr *) &their_addr,
				sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}
