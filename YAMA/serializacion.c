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

void _registrarBloquePlanificacion(t_list * listaNodos, int numBloque, long bytes, DescriptorNodo * nodos, int cantidadNodos);
void _loguearNodos(t_list * listaNodos, int bloques);
void matarMasterGlobal(int socket);
void matarMaster(int socket);

void enviarTablaTransformacion(int socket_master){
	SolicitudFS solFs;
	solFs.tipomensaje = SOLICITUDARCHIVO;
	memset(solFs.ruta, 0, sizeof(char)*255);
	zrecv(socket_master, solFs.ruta, sizeof(char)* 255, 0);

	zsend(sock_fs, &solFs.tipomensaje, sizeof(solFs.tipomensaje), 0);
	zsend(sock_fs, solFs.ruta, sizeof(solFs.ruta), 0);

	int respuesta;
	t_list * listaNodos = list_create();

	zrecv(sock_fs, &respuesta, sizeof(respuesta), 0);
	if (respuesta != 0){
		log_error(logger, "El archivo %s no se encuentra disponible en el FS", solFs.ruta);
		int respuesta = 0;
		cantidadJobs++;
		zsend(socket_master, &cantidadJobs, sizeof(cantidadJobs), 0);
		zsend(socket_master, &respuesta, sizeof(respuesta), 0);
		cabecera = 0;
		return;
	}

	int bloques, i;

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

	_loguearNodos(listaNodos, bloques);

	cantidadJobs++;
	zsend(socket_master, &cantidadJobs, sizeof(cantidadJobs), 0);

	zsend(socket_master, &bloques, sizeof(bloques), 0);

	planificarBloquesYEnviarAMaster(socket_master, bloques, listaNodos);

	// LIBERAR LISTA (hicimos malloc)
}

void _loguearNodos(t_list * listaNodos, int bloques) {

	int i = 1;
	void printNodo(void * nodo) {

		InfoNodo * infoNodo = (InfoNodo *) nodo;
		int j;
		printf("%s tiene %d bloques: ", infoNodo->nombre, dictionary_size(infoNodo->bloques));
		for (j = 0; j < bloques; j++) {

			char numeroBloqueStr[5];
			intToString(j, numeroBloqueStr);
			TamanoBloque * bloq = (TamanoBloque *) dictionary_get(infoNodo->bloques, numeroBloqueStr);

			if (bloq != NULL) {
				printf("%d ", bloq->bloque);
			}
		}
		printf("\n");
		i++;
	}
	list_iterate(listaNodos, printNodo);
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
				en->disponibilidad = trafo->disponibilidad;
				logEntrada(en->masterId, en->jobId, trafo->disponibilidad,(int) trabajoActual(en->nombreNodo) +1, "EN PROCESO", en->nombreNodo, "-", 0,
						"REDUCCION LOCAL", en->archivoTemporal);
			}

			zsend(socket, trafo->archivoTemporal, sizeof(char) * 255, 0);
		}
	}

	zsend(socket, en->archivoTemporal, sizeof(char) *  255, 0);

	list_add(tablaEstado, en);


}

void matarMasterGlobal(int socket){
	int jobId;
	int muerte = FALLOREDGLOBAL;

	zrecv(socket, &jobId, sizeof(jobId), 0);

	bool criterioBusqueda(void * entrada) {
		EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *) entrada;

		return entradaTablaEstado->jobId == jobId &&
			   entradaTablaEstado->etapa == REDUCGLOBAL;
	}

	EntradaTablaEstado * entradaFinalizada = list_find(tablaEstado,
													   criterioBusqueda);

	if (entradaFinalizada != NULL) {
		entradaFinalizada->estado = ERRORYAMA;
		logEntrada(entradaFinalizada->masterId, entradaFinalizada->jobId,
						entradaFinalizada->disponibilidad,
						(int) trabajoActual(entradaFinalizada->nombreNodo), "ERROR    ",
						entradaFinalizada->nombreNodo, "-", 0, "REDUC. GLOBAL",
						entradaFinalizada->archivoTemporal);
		log_error(logger, "Error en reduccion global en nodo %s, Job %d", entradaFinalizada->nombreNodo, jobId);
		cabecera = 0;
	}

	zsend(socket, &muerte, sizeof(int), 0);
}

void matarMaster(int socket){
    int jobId;
    char archivoReducido[255];
	int muerte = FALLOREDLOCAL;

    zrecv(socket, &jobId, sizeof(jobId), 0);
    zrecv(socket, archivoReducido, sizeof(char) * 255, 0);

	bool criterioBusqueda(void * entrada) {
		EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *) entrada;

		return entradaTablaEstado->jobId == jobId &&
			   entradaTablaEstado->etapa == REDUCCIONLOCAL &&
			   strcmp(entradaTablaEstado->archivoTemporal, archivoReducido) == 0;
	}

	EntradaTablaEstado * entradaFinalizada = list_find(tablaEstado,
													   criterioBusqueda);

	if (entradaFinalizada != NULL) {
		entradaFinalizada->estado = ERRORYAMA;

		logEntrada(entradaFinalizada->masterId, entradaFinalizada->jobId,
						entradaFinalizada->disponibilidad,
						(int) trabajoActual(entradaFinalizada->nombreNodo), "ERROR    ",
						entradaFinalizada->nombreNodo, "-", 0, "REDUCCION LOCAL",
						entradaFinalizada->archivoTemporal);
		log_error(logger, "Error en reduccion local en nodo %s, Job %d", entradaFinalizada->nombreNodo, jobId);
		cabecera = 0;
	}

	zsend(socket, &muerte, sizeof(int), 0);
}

void almacenamientoError(int socket){
	int job;
	zrecv(socket, &job, sizeof(job), 0);
	bool criterioBusqueda(void * entrada) {
		EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *) entrada;

		return entradaTablaEstado->jobId == job
				&& entradaTablaEstado->etapa == ALMACENAMIENTOOP;
	}

	EntradaTablaEstado * entradaFinalizada = list_find(tablaEstado,
			criterioBusqueda);

	if (entradaFinalizada != NULL) {
		entradaFinalizada->estado = ERRORYAMA;
		logEntrada(entradaFinalizada->masterId, entradaFinalizada->jobId,
				entradaFinalizada->disponibilidad,
				(int) trabajoActual(entradaFinalizada->nombreNodo), "ERROR    ",
				entradaFinalizada->nombreNodo, "-", 0, "ALMACENAMIENTO",
				entradaFinalizada->archivoTemporal);
		log_error(logger, "Error al almacenar el archivo en Job %d. Master finalizado.", job);
		cabecera = 0;
		int muerte = ERRORALMACENAMIENTO;
		zsend(socket, &muerte, sizeof(int), 0);
	}
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
			matarMaster(socket);
			break;
		case FALLOREDGLOBAL:
			matarMasterGlobal(socket);
			break;
		case TRANSFORMACIONOK:
			transformacionOK(socket);
			break;
		case REDLOCALOK:
			reduccionLocalOK(socket);
			break;
		case REDGLOBALOK:
			reduccionGlobalOk(socket);
			break;
		case ALMACENAMIENTOOK:
			almacenamientoOK(socket);
			break;
		case ERRORALMACENAMIENTO:
			almacenamientoError(socket);
			break;
	}
}

void _registrarBloquePlanificacion(t_list * listaNodos, int numBloque, long bytes, DescriptorNodo * nodos, int cantidadNodos) {
	int i;

	for (i = 0; i < cantidadNodos; ++i) {
		bool criterio(void *nodo) {
			InfoNodo *infoNodo = (InfoNodo *) nodo;

			return strcmp(infoNodo->nombre, nodos[i].nombreNodo) == 0;
		}

		InfoNodo *nodo = (InfoNodo *) list_find(listaNodos, criterio);

		if (nodo == NULL) {
			InfoNodo *nuevoNodo = (InfoNodo *) malloc(sizeof(*nuevoNodo));
			strcpy(nuevoNodo->nombre, nodos[i].nombreNodo);
			nuevoNodo->disponibilidad = disponibilidad_base;
			nuevoNodo->trabajosActuales = 0;
			nuevoNodo->bloques = dictionary_create();

			TamanoBloque *bloque = (TamanoBloque *) malloc(sizeof(*bloque));
			char *numBloqueStr = (char *) malloc(sizeof(char) * 5);
			intToString(numBloque, numBloqueStr);

			bloque->bloque = nodos[i].bloque;
			bloque->bytes = bytes;

			strcpy(nuevoNodo->ip, nodos[i].ip);
			nuevoNodo->puerto = nodos[i].puerto;

			dictionary_put(nuevoNodo->bloques, numBloqueStr, bloque);

			list_add(listaNodos, nuevoNodo);
		} else {
			TamanoBloque *bloque = (TamanoBloque *) malloc(sizeof(*bloque));
			char *numBloqueStr = (char *) malloc(sizeof(char) * 5);
			intToString(numBloque, numBloqueStr);

			bloque->bloque = nodos[i].bloque;
			bloque->bytes = bytes;

			dictionary_put(nodo->bloques, numBloqueStr, bloque);
		}
	}
}
