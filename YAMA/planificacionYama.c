#include "planificacionYama.h"
#include <string.h>

InfoNodo _roundRobin(InfoNodo copia0, InfoNodo copia1);
InfoNodo _weightedRoundRobin(InfoNodo copia0, InfoNodo copia1);

InfoNodo elegirNodo(InfoNodo copia0, InfoNodo copia1){
    if(algoritmoBalanceo == ROUNDROBIN){
        return _roundRobin(copia0, copia1);
    }

    return _weightedRoundRobin(copia0, copia1);
}

InfoNodo _roundRobin(InfoNodo copia0, InfoNodo copia1){
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

InfoNodo _weightedRoundRobin(InfoNodo copia0, InfoNodo copia1){
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
}

int availability(){
	if(algoritmoBalanceo == ROUNDROBIN){
	        return disponibilidad_base;
	    }

	    return disponibilidad_wRR();
	}

//unsigned int disponiblidad_wRR();
