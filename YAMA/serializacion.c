/*
 * seralizacion.c
 *
 *  Created on: 4/9/2017
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <archivos.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sockets.h>
#include "serializacion.h"
#include "servidor.h"
#include "chat.h"
#include <protocoloComunicacion.h>
#include "yama.h"

void mandarOperacionTrafo (int socket, char nombNodo[100], char ip[20], uint16_t puerto, int nroBloque, long bytes);


void enviarTablaTransformacion(int socket_master){
	SolicitudFS solFs;
	solFs.tipomensaje = SOLICITUDARCHIVO;
	memset(solFs.ruta, 0, sizeof(char)*255);
	zrecv(socket_master, solFs.ruta, sizeof(char)* 255, 0);

	zsend(sock_fs, &solFs, sizeof(solFs), 0);

	int respuesta;

	zrecv(sock_fs, &respuesta, sizeof(respuesta), 0);
	if (respuesta != 0){
		//zsend(socket_master, )  -- Mandar a master que aborte la trafo
		log_error(logger, "El archivo %s no pudo no se encuentra disponible", solFs.ruta);
		return;
	}

	int bloques, i;

	zrecv(sock_fs, &bloques, sizeof(bloques), 0);

	zsend(socket_master, &bloques, sizeof(bloques), 0);
	for(i = 0; i < bloques; ++i){
		int copias, j;
		long bytes;

		zrecv(sock_fs, &bytes, sizeof(bytes), 0);
		zrecv(sock_fs, &copias, sizeof(copias), 0);
		DescriptorNodo nodos[copias];

		for (j = 0; j < copias; ++j){
			zrecv(sock_fs, nodos[j].nombreNodo, sizeof(char)*100, 0);
			zrecv(sock_fs, nodos[j].ip, sizeof(char)*20, 0);
			zrecv(sock_fs, &nodos[j].puerto, sizeof(nodos[j].puerto), 0);
			zrecv(sock_fs, &nodos[j].bloque, sizeof(nodos[j].bloque), 0);
		}
		if (copias == 1){
							//FALTA ENVIAR Y ṔLANIFICAR
		}
	}

	int cantidadWorkersEjemplo = 2;

	zsend(socket_master, &cantidadWorkersEjemplo, sizeof(int), 0);



	printf("El nombre del archivo es: %s\n", solFs.ruta);
	while(cantidadWorkersEjemplo--){
		operacionTransformacion op;
		strcpy(op.nombreNodo, "nodo1");
		op.bloque = cantidadWorkersEjemplo +2;
		op.bytes = 10;
		strcpy(op.ip, "127.0.0.1");
		op.puerto = htons(PORTNODO);
		strcpy(op.ruta, "temp/algo.txt");
		zsend(socket_master, &op, sizeof(op), 0);
		printf("nom: %s\nbloq: %d\nbytes: %d\nip: %s\nport: %d\nruta: %s\n",
				op.nombreNodo, op.bloque, op.bytes, op.ip, op.puerto, op.ruta);
	}

}

void enviarSolicitudReduccion(int socket) {
	int i;

	operacionReduccion op;
	strcpy(op.nombreNodo, "nombrePrueba");
	strcpy(op.ip, "127.0.0.1");
	op.puerto = htons(9002);
	op.cantidadTemporales = 4;
	strcpy(op.archivoReducido, "archivoReducido");

	rutaArchivo rutas[op.cantidadTemporales];

	strcpy(&rutas[0], "ruta1");
	strcpy(&rutas[1], "ruta2");
	strcpy(&rutas[2], "ruta3");
	strcpy(&rutas[3], "ruta4");

	for (i = 0; i < op.cantidadTemporales; i++) {
		printf("ruta %s\n", &rutas[i]);
	}

	zsend(socket, &op, sizeof(op), 0);

	while (op.cantidadTemporales--) {
		zsend(socket, &rutas[op.cantidadTemporales], sizeof(rutaArchivo), 0);
	}
}


void manejarDatos(int buf, int socket){
	switch(buf){
	case SOLICITUDJOB:
		enviarTablaTransformacion(socket);
		break;
	case PEDIDOREDUCCION:
		enviarSolicitudReduccion(socket);
		break;
	}
}

void mandarOperacionTrafo (int socket, char nombNodo[100], char ip[20], uint16_t puerto, int nroBloque, long bytes){
	zsend(socket, nombNodo,sizeof(char)*100, 0);
	zsend(socket, ip,sizeof(char)*20, 0);
	zsend(socket, &puerto,sizeof(puerto), 0);
	zsend(socket, &nroBloque,sizeof(nroBloque), 0);
	zsend(socket, &bytes,sizeof(bytes), 0);			//FALTA MANDAR NOMBRE ARCHIVO TEMPORAL, AGREGAR OPERACION A TABLA DE OPERACIONES,
}
