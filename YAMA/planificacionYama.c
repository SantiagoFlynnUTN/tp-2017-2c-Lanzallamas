#include "planificacionYama.h"
#include <string.h>
#include <sys/time.h>
#include <sockets.h>
#include <protocoloComunicacion.h>

int _buscarMax(t_list * nodos);
int _getTareasHistoricas(InfoNodo * nodo);
int _pWl(InfoNodo * nodo, int max);
void _setAvailability(t_list * nodos);
void _sumarTrabajos(t_list * nodos);
void _enviarAMaster(int socket_master, InfoNodo * nodo, InfoNodo * nodoCopia, TamanoBloque * bloque, TipoOperacion operacion);
void _sumarDisponiblidadBase(t_list * listaNodos);
void _sortNodos(t_list * nodos);
void _iniciarTrabajos(t_list * nodos);


void ordenarNodos(t_list * nodos) {
	_sumarTrabajos(nodos);
	_setAvailability(nodos);
	_sortNodos(nodos);
}


InfoNodo * buscarCopia(char numeroBloqueStr[5], t_list * listaNodos){
	int encontrado = 0, i = 0;
	while (!encontrado && i < list_size(listaNodos)) {
		InfoNodo * nodo = (InfoNodo *) list_get(listaNodos, i);
		TamanoBloque * bloque = (TamanoBloque *) dictionary_get(nodo->bloques,
				numeroBloqueStr);
		if (bloque != NULL) {
			encontrado = 1;
			return nodo;
		}
		i++;
	}
	return NULL;
}


void replanificar(int socket){
	usleep(retardoPlanificacion * 1000);
	InfoNodo* nodoCopia = NULL;
	transfError transf;
	zrecv(socket, &transf, sizeof(transf), 0);

	void buscarNodoCopia(void * entrada){
		EntradaTablaEstado * en = (EntradaTablaEstado *) entrada;

		if(strcmp(en->nombreNodo, transf.nombreNodo) == 0 &&
			en->numeroBloque == transf.numBloque &&
			strcmp(en->archivoTemporal, transf.nombreTemp) == 0){
			en->estado = ERRORYAMA;
			nodoCopia = en->nodoCopia;
		}
	}

	list_iterate(tablaEstado, buscarNodoCopia);

	if(nodoCopia!=NULL){
		int tipoOperacion = REPLANIFICACION;
		TamanoBloque * bloque = (TamanoBloque *) malloc(sizeof(*bloque));
		bloque->bloque = transf.numBloque;
		bloque->bytes = transf.bytes;
		zsend(socket, &tipoOperacion, sizeof(tipoOperacion), 0);

		log_info(logger, "\tReplanificando...");

		_enviarAMaster(socket, nodoCopia, NULL, bloque, TRANSFORMACION);
	}else {
		int muerte = FALLOTRANSFORMACION;

		log_error(logger, "No se puede replanificar la transformacion\n");
		zsend(socket, &muerte, sizeof(int), 0);
		cabecera = 0;
	}
}

void planificarBloquesYEnviarAMaster(int socket_master, int bloques, t_list * listaNodos){
	usleep(retardoPlanificacion * 1000);
	int i;
	int clock_ = 0;
	int cantidadNodos = list_size(listaNodos);
	printf("Disponibilidades inicial:\n");
	void printDispI(void*nodo){
		InfoNodo*n=(InfoNodo*) nodo;
		printf("%s: %d\n", n->nombre, n->disponibilidad);
	}
	list_iterate(listaNodos, printDispI);
	cabecera = 0;

	for(i = 0; i < bloques; ++i){
		int inicial = clock_;
		int planificado = 0;
		int fallos = 0;

		while(!planificado){

			InfoNodo * nodo = (InfoNodo *) list_get(listaNodos, clock_), *nodoCopia;
			char numeroBloqueStr[5];
			intToString(i, numeroBloqueStr);

			TamanoBloque * bloque = (TamanoBloque *) dictionary_get(nodo->bloques,numeroBloqueStr);
			nodoCopia = buscarCopia(numeroBloqueStr, listaNodos);

			if (bloque != NULL) {
				if (nodo->disponibilidad == 0) {
					nodo->disponibilidad += disponibilidad_base;
					fallos++;
				} else {
					if (clock_ == inicial && fallos) {
						_sumarDisponiblidadBase(listaNodos);
					}
					_enviarAMaster(socket_master, nodo, nodoCopia, bloque,
							TRANSFORMACION);
					nodo->disponibilidad--;
					planificado = 1;
					dictionary_remove(nodo->bloques, numeroBloqueStr);
				}
			}

			clock_ = (clock_ + 1) % cantidadNodos;
		}
	}
	printf("Disponibilidades final:\n");
	void printDispF(void*nodo){
		InfoNodo*n=(InfoNodo*) nodo;
		printf("%s: %d\n", n->nombre, n->disponibilidad);
	}
	list_iterate(listaNodos, printDispF);
	cabecera = 0;
}

void _enviarAMaster(int socket_master, InfoNodo * nodo, InfoNodo * nodoCopia, TamanoBloque * bloque, TipoOperacion operacion){
	zsend(socket_master, nodo->nombre, sizeof(char)*100, 0);
	zsend(socket_master, nodo->ip, sizeof(char)*20, 0);
	zsend(socket_master, &nodo->puerto, sizeof(nodo->puerto), 0);
	zsend(socket_master, &bloque->bloque, sizeof(bloque->bloque), 0);
	zsend(socket_master, &bloque->bytes, sizeof(bloque->bytes), 0);

	char tempFile[255];
	memset(tempFile, 0, sizeof(char) * 255);

	generarArchivoTemporal(nodo->nombre, tempFile);

	zsend(socket_master, tempFile, 255 * sizeof(char), 0);

	EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *)malloc(sizeof(*entradaTablaEstado));

	entradaTablaEstado->masterId = cantidadJobs;
	entradaTablaEstado->jobId = cantidadJobs;
	entradaTablaEstado->estado = ENPROCESO;
	strcpy(entradaTablaEstado->nombreNodo, nodo->nombre);
	entradaTablaEstado->numeroBloque = bloque->bloque;
	entradaTablaEstado->etapa = operacion;
	strcpy(entradaTablaEstado->archivoTemporal, tempFile);
	strcpy(entradaTablaEstado->ip, nodo->ip);
	entradaTablaEstado->puerto = nodo->puerto;
	entradaTablaEstado->nodoCopia = nodoCopia;


	if (!cabecera) {
		printf("\nMaster\tJobId\tDisp\tCarga\tEstado\t\tNodo\tBloque\tEtapa\t\tTemporal\n");
		cabecera = 1;
	}
	printf("%d\t%d\t%d\t%d\t%s\t%s\t%d\t%s\t%s\n", entradaTablaEstado->masterId,
			entradaTablaEstado->jobId, nodo->disponibilidad,
			nodo->trabajosActuales, "EN PROCESO",
			entradaTablaEstado->nombreNodo, entradaTablaEstado->numeroBloque,
			"TRANSFORMACION", entradaTablaEstado->archivoTemporal);

	list_add(tablaEstado, entradaTablaEstado);
}

void generarArchivoTemporal(char * nombre, char * file){
	struct timeval tv;
	gettimeofday(&tv,NULL);

	sprintf(file, "temp/%s-%ld.%06ld", nombre, tv.tv_sec, tv.tv_usec);
}

void _setAvailability(t_list * nodos){
	if(algoritmoBalanceo == WEIGHTEDCLOCK){
		int max = _buscarMax(nodos);
		void availability(void * nodo){
			InfoNodo * infoNodo = (InfoNodo *) nodo;
			infoNodo->disponibilidad += _pWl(infoNodo, max);
		}
		list_iterate(nodos, availability);
	}
}

void _sumarTrabajos(t_list * nodos){
	void sumarTrabajo(void * entrada){
		EntradaTablaEstado * en = (EntradaTablaEstado *) entrada;
		if(en->estado == ENPROCESO){
			bool buscarEnListaNodos(void * nodo){
				InfoNodo * in = (InfoNodo *) nodo;
				return strcmp(en->nombreNodo, in->nombre) == 0;
			}
			InfoNodo * nodo = list_find(nodos,buscarEnListaNodos);
			printf("Sumo a: %s\n", nodo->nombre);
			if(nodo != NULL){
				nodo->trabajosActuales ++;
			}
		}
	}
	list_iterate(tablaEstado,sumarTrabajo);
}

int _buscarMax(t_list * nodos){
	int max = 0;
	int cantNodos = list_size(nodos);
	int i;
	for(i = 0; i < cantNodos; ++i){
		InfoNodo * nodo = (InfoNodo *) list_get(nodos, i);
		if(nodo->trabajosActuales > max){
			max = nodo->trabajosActuales;
		}
	}
	return max;
}

int _pWl(InfoNodo * nodo, int max){
	return max - nodo->trabajosActuales;
}

void _sumarDisponiblidadBase(t_list * listaNodos){
	void sumarDisponibilidad(void * nodo){
		InfoNodo * in = (InfoNodo *) nodo;
		in->disponibilidad += disponibilidad_base;
	}
	list_iterate(listaNodos, sumarDisponibilidad);
}

void _sortNodos(t_list * nodos){
	bool _ordenarPorDisponibilidad(void * n1, void * n2){
		InfoNodo * nodo1 = (InfoNodo *) n1,
				* nodo2 = (InfoNodo *) n2;

		if(nodo1->disponibilidad == nodo2->disponibilidad){
			return _getTareasHistoricas(nodo1) <  _getTareasHistoricas(nodo2);
		}

		return nodo1->disponibilidad > nodo2->disponibilidad;
	}

	list_sort(nodos, _ordenarPorDisponibilidad);
}

int _getTareasHistoricas(InfoNodo * nodo){
	int tareas = 0;
	void sumarTrabajoHistoricos(void * entrada){
		EntradaTablaEstado * en = (EntradaTablaEstado *) entrada;

		if(en->estado != ENPROCESO && strcmp(en->nombreNodo, nodo->nombre) == 0){
			tareas++;
		}
	}

	list_iterate(tablaEstado, sumarTrabajoHistoricos);

	return tareas;
}
