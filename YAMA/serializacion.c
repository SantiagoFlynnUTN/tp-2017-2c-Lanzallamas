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
void _registrarTransformacion(int socket, char nombNodo[100], char ip[20], uint16_t puerto, int nroBloque, long bytes);
void _registrarBloquePlanificacion(t_list * listaNodos, int numBloque, long bytes, DescriptorNodo * nodos, int cantidadNodos);

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
	t_list * listaNodos = list_create();

	zrecv(sock_fs, &bloques, sizeof(bloques), 0);

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

		_registrarBloquePlanificacion(listaNodos, i, bytes, nodos, copias);
		/*if (copias == 1){
			mandarOperacionTrafo(socket_master, nodos[0].nombreNodo, nodos[0].ip, nodos[0].puerto, nodos[0].bloque, bytes);
		} else if(copias == 2){
			mandarOperacionTrafo(socket_master, nodos[0].nombreNodo, nodos[0].ip, nodos[0].puerto, nodos[0].bloque, bytes);
			// planificar y enviar
		}*/
	}

	zsend(socket_master, &bloques, sizeof(bloques), 0);
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

	strcpy(rutas[0].ruta, "ruta1");
	strcpy(rutas[1].ruta, "ruta2");
	strcpy(rutas[2].ruta, "ruta3");
	strcpy(rutas[3].ruta, "ruta4");

	for (i = 0; i < op.cantidadTemporales; i++) {
		log_info(logger, "ruta %s\n", &rutas[i]);
	}

	zsend(socket, &op, sizeof(op), 0);

	while (op.cantidadTemporales--) {
		zsend(socket, &rutas[op.cantidadTemporales], sizeof(rutaArchivo), 0);
	}
}


void manejarDatos(int buf, int socket){
	switch(buf) {
		case SOLICITUDJOB:
			enviarTablaTransformacion(socket);
			break;
		case PEDIDOREDUCCION:
			enviarSolicitudReduccion(socket);
			break;
		case FALLOTRANSFORMACION:
			break;
		case FALLOREDLOCAL:
			break;
	}
}

void mandarOperacionTrafo(int socket, char nombNodo[100], char ip[20], uint16_t puerto, int nroBloque, long bytes){
	zsend(socket, nombNodo,sizeof(char)*100, 0);
	zsend(socket, ip,sizeof(char)*20, 0);
	zsend(socket, &puerto,sizeof(puerto), 0);
	zsend(socket, &nroBloque,sizeof(nroBloque), 0);
	zsend(socket, &bytes,sizeof(bytes), 0);
	zsend(socket, "/temp/j1n1b8", 255 * sizeof(char), 0);

}

void _registrarTransformacion(int socket, char nombNodo[100], char ip[20], uint16_t puerto, int nroBloque, long bytes){
	EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *) malloc(sizeof(*entradaTablaEstado));
	entradaTablaEstado->jobId = 0;
	entradaTablaEstado->masterId = 0;
	strcpy(entradaTablaEstado->nombreNodo, nombNodo);
	entradaTablaEstado->numeroBloque = nroBloque;
	entradaTablaEstado->etapa = TRANSFORMACION;
	strcpy(entradaTablaEstado->archivoTemporal, "/temp/j1n1b8");

	list_add(tablaEstado, entradaTablaEstado);
}

void _registrarBloquePlanificacion(t_list * listaNodos, int numBloque, long bytes, DescriptorNodo * nodos, int cantidadNodos){
	int i;

	for(i = 0; i < cantidadNodos; ++i){
		bool criterio(void * nodo){
			InfoNodo * infoNodo = (InfoNodo *) nodo;

			return strcmp(infoNodo->nombre, nodos[i].nombreNodo) == 0;
		}

		InfoNodo * nodo = (InfoNodo *) list_find(listaNodos, criterio);

		if(nodo == NULL){
			InfoNodo * nuevoNodo = (InfoNodo *) malloc(sizeof(*nuevoNodo));
			strcpy(nuevoNodo->nombre,nodos[i].nombreNodo);
			nuevoNodo->disponibilidad = disponibilidad_base;
			nuevoNodo->bloques = dictionary_create();

			TamanoBloque * bloque = (TamanoBloque *)malloc(sizeof(*bloque));
			char * numBloqueStr = (char *) malloc(sizeof(char) * 5);
			intToString(numBloque, numBloqueStr);

			bloque->bloque = nodos[i].bloque;
			bloque->bytes = bytes;

			dictionary_put(nuevoNodo->bloques, numBloqueStr, bloque);

			list_add(listaNodos, nuevoNodo);
		}else{
			TamanoBloque * bloque = (TamanoBloque *)malloc(sizeof(*bloque));
			char * numBloqueStr = (char *) malloc(sizeof(char) * 5);
			intToString(numBloque, numBloqueStr);

			bloque->bloque = nodos[i].bloque;
			bloque->bytes = bytes;

			dictionary_put(nodo->bloques, numBloqueStr, bloque);
		}
	}
}

