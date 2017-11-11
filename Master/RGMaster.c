#include <stdint.h>
#include <protocoloComunicacion.h>
#include <sockets.h>
#include "cliente.h"
#include "RGMaster.h"
#include "master.h"

void _conexionReduccionWorker(int *sockfd, OperacionReduccionGlobal op);

void reduccionGlobal(int socket_yama){
	int cantidad;

	int i = 0;

	zrecv(socket_yama, &cantidad, sizeof(cantidad), 0);

	OperacionReduccionGlobal operacionReduccion[cantidad - 1];
	OperacionReduccionGlobal encargado;

	char archivoFinal[255];

	int salteos = 0;

	for(i = 0; i < cantidad; ++i){
		int esEncargado;

		zrecv(socket_yama, &esEncargado, sizeof(esEncargado), 0);

		if(esEncargado == 1){
			zrecv(socket_yama, encargado.nombreNodo, sizeof(char) * 100, 0);
			zrecv(socket_yama, encargado.ip, sizeof(char) * 20, 0);
			zrecv(socket_yama, &encargado.puerto, sizeof(encargado.puerto), 0);
			zrecv(socket_yama, encargado.archivoReducido, sizeof(char) * 255, 0);

			salteos++;

			log_info(logger, "NODO:%s\nIP:%s\nPUERTO:%d\nARCHIVO:%s\nENCARGADO:SI\n", encargado.nombreNodo, encargado.ip, encargado.puerto, encargado.archivoReducido);
		}else{
			zrecv(socket_yama, operacionReduccion[i - salteos].nombreNodo, sizeof(char) * 100, 0);
			zrecv(socket_yama, operacionReduccion[i - salteos].ip, sizeof(char) * 20, 0);
			zrecv(socket_yama, &operacionReduccion[i - salteos].puerto, sizeof(encargado.puerto), 0);
			zrecv(socket_yama, operacionReduccion[i - salteos].archivoReducido, sizeof(char) * 255, 0);

			log_info(logger, "NODO:%s\nIP:%s\nPUERTO:%d\nARCHIVO:%s\nENCARGADO:NO\n", operacionReduccion[i - salteos].nombreNodo, operacionReduccion[i - salteos].ip, operacionReduccion[i - salteos].puerto, operacionReduccion[i - salteos].archivoReducido);
		}
	}
    cantidad--; // no cuento el encargado

	zrecv(socket_yama, archivoFinal, sizeof(char) * 255, 0);

	printf("ARCHIVO FINAL:%s", archivoFinal);

    int socketNodo;
    int tipoMensaje = SOLICITUDREDUCCIONGLOBAL;

    _conexionReduccionWorker(&socketNodo, encargado);

    zsend(socketNodo, &tipoMensaje, sizeof(int), 0);
    zsend(socketNodo, encargado.archivoReducido, sizeof(char) * 255, 0);
    zsend(socketNodo, &cantidad, sizeof(cantidad), 0);

    for(i = 0; i < cantidad; ++i){
        zsend(socketNodo, encargado.nombreNodo, sizeof(char) * 100, 0);
        zsend(socketNodo, encargado.ip, sizeof(char) * 20, 0);
        zsend(socketNodo, &encargado.puerto, sizeof(encargado.puerto), 0);
        zsend(socketNodo, encargado.archivoReducido, sizeof(char) * 255, 0);
    }

    int status;
    if(recv(socketNodo, &status, sizeof(int), 0) == -1 || status != 0){
        int mensajeError = REDGLOBAL;
        log_error(logger, "Fallo reduccion global en nodo %s\n", encargado.nombreNodo);
        zsend(socket_yama, &mensajeError, sizeof(int), 0);
    }else{
        int mensajeOK = REDGLOBALOK;
        log_info(logger, "worker %d finalizó reduccion\n", socketNodo);
        zsend(socket_yama, &mensajeOK, sizeof(int), 0);
    }

    zsend(socket_yama, &jobId, sizeof(jobId), 0);
}

void _conexionReduccionWorker(int *sockfd, OperacionReduccionGlobal op) {

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