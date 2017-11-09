#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sockets.h>
#include "yama.h"
#include <protocoloComunicacion.h>
#include "planificacionYama.h"

int calcularIndiceConValorMaximo(int * array, int cantidad);

void reduccionGlobal(int socket, int jobId){

	bool criterioFilter(void * entrada){
		EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *)entrada;

		return entradaTablaEstado->jobId == jobId &&
			   entradaTablaEstado->etapa == REDUCCIONLOCAL;
	}

	t_list * reducciones = list_filter(tablaEstado, criterioFilter);
	int cantidad = list_size(reducciones);

	int trabajosActuales[cantidad];
	memset(trabajosActuales, 0, cantidad * sizeof(int));

	void iterateFunction(void * entrada){
		EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *)entrada;

		if(entradaTablaEstado->estado == ENPROCESO){
			int i;
			for(i = 0; i < cantidad; ++i){
				EntradaTablaEstado * nodo = (EntradaTablaEstado *) list_get(reducciones, i);

				if(strcmp(nodo->nombreNodo, entradaTablaEstado->nombreNodo) == 0){
					trabajosActuales[i]++;
				}
			}
		}
	}

	int maxIndex = calcularIndiceConValorMaximo(trabajosActuales, cantidad);

	int tipoMensaje = SOLICITUDREDUCCIONGLOBAL;

	zsend(socket, &tipoMensaje, sizeof(tipoMensaje), 0);

	zsend(socket, &cantidad, sizeof(cantidad), 0);

	int i;

	EntradaTablaEstado * en = (EntradaTablaEstado *) malloc(sizeof(*en));

	for(i = 0; i < cantidad; ++i){
		EntradaTablaEstado * entradaNodo = list_get(reducciones, i);

		int esEncargado = i == maxIndex ? 1 : 0;

		zsend(socket, &esEncargado, sizeof(esEncargado), 0);
		zsend(socket, entradaNodo->nombreNodo, sizeof(char) * 100, 0);
		zsend(socket, entradaNodo->ip, sizeof(char) * 20, 0);
		zsend(socket, &entradaNodo->puerto, sizeof(entradaNodo->puerto), 0);
		zsend(socket, entradaNodo->archivoTemporal, sizeof(char) * 255, 0);


		if(esEncargado){
			en->puerto = entradaNodo->puerto;
			en->estado = ENPROCESO;
			en->etapa = REDUCGLOBAL;
			en->numeroBloque = 0;
			en->jobId = entradaNodo->jobId;
			en->masterId = entradaNodo->masterId;
			strcpy(en->nombreNodo, entradaNodo->nombreNodo);
			strcpy(en->ip, entradaNodo->ip);
			generarArchivoTemporal(en->nombreNodo, en->archivoTemporal);
		}
	}

	zsend(socket, en->archivoTemporal, sizeof(char) * 255, 0);

	list_add(tablaEstado, en);

	log_info(logger, "\nMasterId\tJobId\tEstado\t\tNodo\tBloque\tEtapa\t\t\tTemporal\n"
					 "%d\t\t%d\t%s\t%s\t%d\t%s\t%s",
			 en->masterId,
			 en->jobId,
			 "EN PROCESO",
			 en->nombreNodo,
			 en->numeroBloque,
			 "REDUCCION GLOBAL",
			 en->archivoTemporal);
}

int calcularIndiceConValorMaximo(int * array, int cantidad){
	int i;
	int max = *array;
	int index = 0;

	for(i = 1; i < cantidad; ++i){
		if(array[i] > max){
			index = i;
			max = array[i];
		}
	}

	return index;
}