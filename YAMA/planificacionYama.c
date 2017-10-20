#include "planificacionYama.h"
#include <string.h>

InfoNodo _clock(InfoNodo copia0, InfoNodo copia1);
InfoNodo _weightedClock(InfoNodo copia0, InfoNodo copia1);

InfoNodo elegirNodo(InfoNodo copia0, InfoNodo copia1){
    if(algoritmoBalanceo == CLOCK){
        return _clock(copia0, copia1);
    }

    return _weightedRoundRobin(copia0, copia1);
}

/*InfoNodo _clock(InfoNodo copia0, InfoNodo copia1){
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

void planificacion(InfoNodo *nodos, DescriptorBloque *bloques){
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



void setAvailability(InfoNodo nodos){
	if(algoritmoBalanceo == CLOCK){
	        while(nodos != NULL){
	        	nodos.disponibilidad = disponibilidad_base;
	        }
	    }
	if(algoritmoBalanceo == WEIGHTEDCLOCK){
			while(nodos != NULL){
				nodos.disponibilidad = disponibilidad_base + pWL(nodos);
			}
	}
	}



//unsigned int disponiblidad_wRR();
