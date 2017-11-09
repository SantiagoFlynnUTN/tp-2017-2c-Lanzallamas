#include "respuestaTransformacion.h"
#include <sockets.h>
#include "yama.h"
#include <string.h>
#include "serializacion.h"
#include "RGYAMA.h"

void transformacionOK(int socket){
    int jobId;
    char tempFile[255];
    zrecv(socket, &jobId, sizeof(jobId), 0);
    zrecv(socket, tempFile, sizeof(char) * 255, 0);

    bool criterioBusqueda(void * entrada){
        EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado * )entrada;

        return strcmp(tempFile, entradaTablaEstado->archivoTemporal) == 0;
    }

    EntradaTablaEstado * entradaFinalizada = list_find(tablaEstado, criterioBusqueda);

    if(entradaFinalizada != NULL){
        entradaFinalizada->estado = FINALIZADO;

        bool criterioFilter(void * entrada){
            EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *)entrada;

            return entradaTablaEstado->jobId == jobId &&
                   entradaTablaEstado->etapa == TRANSFORMACION &&
                   strcmp(entradaTablaEstado->nombreNodo, entradaFinalizada->nombreNodo) == 0;
        }

        t_list * transformaciones = list_filter(tablaEstado, criterioFilter);

        bool estanFinalizadas(void * entrada){
            EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *)entrada;

            return entradaTablaEstado->estado == FINALIZADO;
        }

        if(list_all_satisfy(transformaciones, estanFinalizadas)){
            enviarSolicitudReduccion(socket, transformaciones);
        }
    }
}

void reduccionLocalOK(int socket){
    int jobId;
    char tempFile[255];
    zrecv(socket, &jobId, sizeof(jobId), 0);
    zrecv(socket, tempFile, sizeof(char) * 255, 0);

    bool criterioBusqueda(void * entrada){
        EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado * )entrada;

        return strcmp(tempFile, entradaTablaEstado->archivoTemporal) == 0;
    }

    EntradaTablaEstado * entradaFinalizada = list_find(tablaEstado, criterioBusqueda);

    if(entradaFinalizada != NULL){
        entradaFinalizada->estado = FINALIZADO;

        bool criterioFilter(void * entrada){
            EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *)entrada;

            return entradaTablaEstado->jobId == jobId &&
                   entradaTablaEstado->etapa == REDUCCIONLOCAL;
        }

        t_list * reducciones = list_filter(tablaEstado, criterioFilter);

        bool estanFinalizadas(void * entrada){
            EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *)entrada;

            return entradaTablaEstado->estado == FINALIZADO;
        }

        if(list_all_satisfy(reducciones, estanFinalizadas)){
            reduccionGlobal(socket, jobId);
        }
    }
}