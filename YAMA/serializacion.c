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
#include "planificacionYama.h"
#include "respuestaTransformacion.h"

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
	}

	ordenarNodos(listaNodos);

	cantidadJobs++;
	zsend(socket_master, &cantidadJobs, sizeof(cantidadJobs), 0);

	zsend(socket_master, &bloques, sizeof(bloques), 0);

	planificarBloquesYEnviarAMaster(socket_master, bloques, listaNodos);

	// LIBERAR LISTA (hicimos malloc)
}

void enviarSolicitudReduccion(int socket, t_list * transformacionesRealizadas){
	int entradas = list_size(transformacionesRealizadas);
	int i;
	int tipoMensaje = SOLICITUDREDUCCIONLOCAL;

	zsend(socket, &tipoMensaje, sizeof(tipoMensaje), 0);

	EntradaTablaEstado * en = (EntradaTablaEstado *) malloc(sizeof(*en));

	for(i = 0; i < entradas; ++i){
		EntradaTablaEstado * trafo = (EntradaTablaEstado *) list_get(transformacionesRealizadas, i);

		if(trafo != NULL){
			if(i == 0){
				zsend(socket, trafo->nombreNodo, sizeof(char) * 100, 0);
				zsend(socket, trafo->ip, sizeof(char) * 20, 0);
				zsend(socket, &trafo->puerto, sizeof(trafo->puerto), 0);
				zsend(socket, &entradas, sizeof(entradas), 0);
				generarArchivoTemporal(trafo->nombreNodo, en->archivoTemporal);
				strcpy(en->nombreNodo, trafo->nombreNodo);
				strcpy(en->ip, trafo->ip);
				en->puerto = trafo->puerto;
				en->estado = ENPROCESO;
				en->etapa = REDUCCIONLOCAL;
				en->numeroBloque = 0;
				en->jobId = trafo->jobId;
				en->masterId = trafo->masterId;
			}

			zsend(socket, trafo->archivoTemporal, sizeof(char) * 255, 0);
		}
	}

	zsend(socket, en->archivoTemporal, sizeof(char) *  255, 0);

	list_add(tablaEstado, en);

	log_info(logger, "\nMasterId\tJobId\tEstado\t\tNodo\tBloque\tEtapa\t\tTemporal\n"
					 "%d\t\t%d\t%s\t%s\t%d\t%s\t%s",
			 en->masterId,
			 en->jobId,
			 "EN PROCESO",
			 en->nombreNodo,
			 en->numeroBloque,
			 "REDUCCION LOCAL",
			 en->archivoTemporal);
}


void manejarDatos(int buf, int socket){
	switch(buf) {
		case SOLICITUDJOB:
			enviarTablaTransformacion(socket);
			break;
		case FALLOTRANSFORMACION:
			replanificar(socket);
			break;
		case FALLOREDLOCAL:
			break;
		case TRANSFORMACIONOK:
			transformacionOK(socket);
			break;
		case REDLOCALOK:
			reduccionLocalOK(socket);
			break;
	}
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

			strcpy(nuevoNodo->ip, nodos[i].ip);
			nuevoNodo->puerto = nodos[i].puerto;

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
