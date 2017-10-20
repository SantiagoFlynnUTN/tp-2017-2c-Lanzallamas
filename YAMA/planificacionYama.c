#include "planificacionYama.h"
#include <string.h>

/*InfoNodo _clock(InfoNodo copia0, InfoNodo copia1);
InfoNodo _weightedClock(InfoNodo copia0, InfoNodo copia1);

InfoNodo elegirNodo(InfoNodo copia0, InfoNodo copia1){
    if(algoritmoBalanceo == CLOCK){
        return _clock(copia0, copia1);
    }

    return _weightedRoundRobin(copia0, copia1);
}

InfoNodo _clock(InfoNodo copia0, InfoNodo copia1){
    int index = list_size(tablaEstado);

    while(index--){
        EntradaTablaEstado * entrada = (EntradaTablaEstado *) list_get(tablaEstado, index);

        if(strcmp(entrada->nombreNodo, copia0.nombre) == 0){
            return copia1;
        } else if(strcmp(entrada->nombreNodo, copia1.nombre) == 0){
            return copia0;
        }
    }

    return copia0;
}

InfoNodo _weightedClock(InfoNodo copia0, InfoNodo copia1){
    int contadorNodo0 = 0,
        contadorNodo1 = 0,
        entradas = list_size(tablaEstado),
        i;

    for(i = 0; i < entradas; ++i){
        EntradaTablaEstado * entrada = (EntradaTablaEstado *) list_get(tablaEstado, i);

        if(strcmp(entrada->nombreNodo, copia0.nombre) == 0){
            contadorNodo0++;
        } else if(strcmp(entrada->nombreNodo, copia1.nombre) == 0){
            contadorNodo1++;
        }
    }

    return contadorNodo0 <= contadorNodo1 ? copia0 : copia1;
}*/

InfoNodo *nodos = list_create();

void planificacion(nodos, DescriptorBloque *bloques){
	setAvailability(nodos);
	setTareasRealizadas(nodos);
	sortByAvailability(nodos);
	sortByTareasRealizadas(nodos);
	pClock = nodos;
	while (bloques != NULL){
		while (!estaEnNodo(bloques, &pClock));
		bloques = bloques->siguiente;
	}

}

//FUNCIONES AUXILIARES//

int estaEnNodo(DescriptorBloque bloque, InfoNodo *pClock){
	if(bloque.copia0 == pClock->nombre || bloque.copia1 == pClock->nombre){
		pClock = pClock->siguiente;
		return 1;
	}
	pClock = pClock->siguiente;
	return 0;
}



void setAvailability(nodos){
	if(algoritmoBalanceo == CLOCK){
		//faltaria hacer que auxiliar apunte al primero de la lista
	        while(auxiliar != NULL){
	        	auxiliar.disponibilidad = disponibilidad_base;
	        	auxiliar = auxiliar.siguiente;
	        }
	    }
	if(algoritmoBalanceo == WEIGHTEDCLOCK){
			while(auxiliar != NULL){
				auxiliar.disponibilidad = disponibilidad_base + pWL(auxiliar);
				auxiliar = auxiliar.siguiente;
			}
	}
	}

void wL(InfoNodo nodo){

}

int pWl(InfoNodo nodo){
	wL(nodo);
	return wlMax - nodo.trabajoActual;
}

void setTareasRealizadas(InfoNodo nodos){
	if(tablaEstado != NULL){

	}

}


void sortByAvailability(nodos){
	int i, j, k;
	InfoNodo nodoAux;
	//apuntar auxiliar al primero de la lista
	for (i = 0; i < (list_size(nodos)-1); ++i) {
		for(j = 0; j < (list_size(nodos)-1); ++j){
					if(auxiliar.disponibilidad < auxiliar.siguiente.disponibilidad){
						nodoAux = auxiliar;
						nodoAux.siguiente = auxiliar.siguiente.siguiente;
						auxiliar = auxiliar.siguiente;
						auxiliar.siguiente = nodoAux;
					}
				}
	}
}

void sortByTareasRealizadas(nodos){
	int i, j, k;
		InfoNodo nodoAux;
		//apuntar auxiliar al primero de la lista
		for (i = 0; i < (list_size(nodos)-1); ++i) {
			for(j = 0; j < (list_size(nodos)-1); ++j){
						if(auxiliar.tareasHistoricas > auxiliar.siguiente.tareasHistoricas){
							nodoAux = auxiliar;
							nodoAux.siguiente = auxiliar.siguiente.siguiente;
							auxiliar = auxiliar.siguiente;
							auxiliar.siguiente = nodoAux;
						}
					}
		}
}
