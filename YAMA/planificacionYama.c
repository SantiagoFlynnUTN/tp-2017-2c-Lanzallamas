#include "planificacionYama.h"
#include <string.h>


void planificacion(t_list * nodos, int cantBloques, int socketMaster){
	setAvailability(nodos);
	sumarTrabajos(nodos);
	sortByAvailability(nodos);
	sortByTareasRealizadas(nodos);
	/*pClock = nodos;
	while (bloques != NULL){
		while (!estaEnNodo(bloques, &pClock));
		bloques = bloques->siguiente;
	}*/

}

//FUNCIONES AUXILIARES//

void setAvailability(t_list * nodos){
	if(algoritmoBalanceo == WEIGHTEDCLOCK){

		int max = _buscarMax(nodos);
		void availability(void * nodo){
			InfoNodo * infoNodo = (InfoNodo *) nodo;
			infoNodo->disponibilidad += pWl(infoNodo, max);
		}
		list_iterate(nodos, availability);
	}
}

void sumarTrabajos(t_list * nodos){
	void sumarTrabajo(void * entrada){
		EntradaTablaEstado * en = (EntradaTablaEstado *) entrada;
		if(en->estado == ENPROCESO){
			bool buscarEnListaNodos(void * nodo){
				InfoNodo * in = (InfoNodo *) nodo;
				return strcmp(en->nombreNodo, in->nombre) == 0;
			}
			InfoNodo * nodo = list_find(nodos,buscarEnListaNodos);
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

int pWl(InfoNodo * nodo, int max){
	return max - nodo->trabajosActuales;
}


void sortByAvailability(t_list * nodos){
	list_sort(nodos, _ordenarPorDisponibilidad);
}

void sortByTareasRealizadas(t_list * nodos){
}

bool _ordenarPorDisponibilidad(void * n1, void * n2){
		InfoNodo * nodo1 = (InfoNodo *) n1,
				* nodo2 = (InfoNodo *) n2;

		return strcmp(nodo1->disponibilidad, nodo2->disponibilidad) >= 0;
	}
