#include "respuestaTransformacion.h"
#include <sockets.h>
#include "yama.h"
#include <string.h>
#include "serializacion.h"
#include "RGYAMA.h"
#include <protocoloComunicacion.h>

void _enviarOperacionAlmacenamiento(int socket, EntradaTablaEstado * reduccion);

void almacenamientoOK(int socket){
    int jobId;

    zrecv(socket, &jobId, sizeof(jobId), 0);

    bool criterioBusqueda(void * entrada){
        EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado * )entrada;

        return entradaTablaEstado->jobId == jobId && entradaTablaEstado->etapa == ALMACENAMIENTOOP;
    }

    EntradaTablaEstado * entradaFinalizada = list_find(tablaEstado, criterioBusqueda);

    if(entradaFinalizada != NULL) {
        entradaFinalizada->estado = FINALIZADO;
        log_info(logger, "El job %d ha sido finalizado", jobId);
    }
}

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
    }else{
        log_error(logger, "NO ENCONTRO LA Transformacion\njob: %d\ntempFile: %s\n", jobId, tempFile);
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

            return entradaTablaEstado->jobId == jobId;
        }

        t_list * reducciones = list_filter(tablaEstado, criterioFilter);

        bool estanFinalizadas(void * entrada){
            EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado *)entrada;

            return entradaTablaEstado->estado == FINALIZADO ||
                   (entradaTablaEstado->etapa == TRANSFORMACION && entradaTablaEstado->estado == ERRORYAMA);
        }

        if(list_all_satisfy(reducciones, estanFinalizadas)){
            reduccionGlobal(socket, jobId);
        }
    }else{
        log_error(logger, "NO ENCONTRO LA Reduccion Local\njob: %d\ntempFile: %s\n", jobId, tempFile);
    }
}

void reduccionGlobalOk(int socket){
    int jobId;

    zrecv(socket, &jobId, sizeof(jobId), 0);

    bool criterioBusqueda(void * entrada){
        EntradaTablaEstado * entradaTablaEstado = (EntradaTablaEstado * )entrada;

        return entradaTablaEstado->jobId == jobId && entradaTablaEstado->etapa == REDUCGLOBAL;
    }

    EntradaTablaEstado * reduccionGlobal = list_find(tablaEstado, criterioBusqueda);

    if(reduccionGlobal != NULL){
        reduccionGlobal->estado = FINALIZADO;

        _enviarOperacionAlmacenamiento(socket, reduccionGlobal);
    }else{
        log_error(logger, "NO ENCONTRO LA Reduccion Global\njob: %d\n", jobId);
    }
}

void _enviarOperacionAlmacenamiento(int socket, EntradaTablaEstado * reduccion){
    EntradaTablaEstado * en = (EntradaTablaEstado *) malloc(sizeof(*en));

    en->puerto = reduccion->puerto;
    en->estado = ENPROCESO;
    en->etapa = ALMACENAMIENTOOP;
    en->numeroBloque = 0;
    en->jobId = reduccion->jobId;
    en->masterId = reduccion->masterId;
    strcpy(en->nombreNodo, reduccion->nombreNodo);
    strcpy(en->ip, reduccion->ip);

    memset(en->archivoTemporal, 0, sizeof(char) * 255);

    list_add(tablaEstado, en);

    log_info(logger, "\nMasterId\tJobId\tEstado\t\tNodo\tBloque\tEtapa\t\t\tTemporal\n"
                     "%d\t\t%d\t%s\t%s\t%d\t%s\t%s",
             en->masterId,
             en->jobId,
             "EN PROCESO",
             en->nombreNodo,
             en->numeroBloque,
             "ALMACENAMIENTO",
             en->archivoTemporal);

    int almacenamiento = SOLICITUDALMACENAMIENTO;
    zsend(socket, &almacenamiento, sizeof(almacenamiento), 0);
    zsend(socket, en->nombreNodo, sizeof(char) * 100,  0);
    zsend(socket, en->ip, sizeof(char) * 20, 0);
    zsend(socket, &en->puerto, sizeof(en->puerto), 0);
    zsend(socket, reduccion->archivoTemporal, sizeof(char) * 255, 0);
}