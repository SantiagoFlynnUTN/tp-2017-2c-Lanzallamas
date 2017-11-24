#include "recepcionBloques.h"
#include "envioBloques.h"
#include <sys/socket.h>
#include "conexionesFileSystem.h"
#include <string.h>

int _guardarBloque(DescriptorNodo * nodo, int bloque, long bytes, FILE * archivo);
int _seleccionarNodoLectura(Bloque * bloque, DescriptorNodo ** nodo);

int obtenerArchivo(char * rutaArchivoFS, char * rutaArchivoFinal){
    Archivo * descriptorArchivo = (Archivo *) dictionary_get(archivos, rutaArchivoFS);

    if(descriptorArchivo == NULL){
        log_error(logger, "El archivo %s no existe en YAMAFS\n", rutaArchivoFS);
        return -1;
    }

    FILE * archivoFinal = fopen(rutaArchivoFinal, "w");

    if(archivoFinal == NULL){
        log_error(logger, "No se puede crear el archivo %s\n", rutaArchivoFinal);
        return -1;
    }

    int i;
    int bloques = list_size(descriptorArchivo->bloques);

    for(i = 0; i < bloques; ++i){
        Bloque * bloque = (Bloque *) list_get(descriptorArchivo->bloques, i);

        DescriptorNodo * descriptorNodo = NULL;

        int numBloque = _seleccionarNodoLectura(bloque, &descriptorNodo);

        if(numBloque == -1){
            log_error(logger, "El bloque %d del archivo %s no tiene copias disponibles\n", i, rutaArchivoFS);
            return -1;
        }

        int bloqueGuardado = _guardarBloque(descriptorNodo, numBloque, bloque->descriptor.bytes, archivoFinal);

        if(bloqueGuardado != 0){
            fclose(archivoFinal);
            remove(rutaArchivoFinal);
            return -1;
        }
    }

    fclose(archivoFinal);
    return 0;
}

int _guardarBloque(DescriptorNodo * nodo, int bloque, long bytes, FILE * archivo){
    int mensaje = GETBLOQUE;
    char contenidoBloque[MB];

    if (send(nodo->socket, &mensaje, sizeof(mensaje), 0) == -1 ||
        send(nodo->socket, &bloque, sizeof(bloque), 0) == -1){
        log_error(logger, "No puedo solicitar el bloque al nodo %s\n", nodo->nombreNodo);
        return -1;
    }

    printf("Recibiendo un bloque de %s\n", nodo->nombreNodo);

    if(recv(nodo->socket, contenidoBloque, sizeof(char) * MB, MSG_WAITALL) <= 0){
        log_error(logger, "Error recibiendo el bloque del nodo %s\n", nodo->nombreNodo);
        return -1;
    }

    if(fwrite(contenidoBloque, sizeof(char) * bytes, 1, archivo) != 1){
        log_error(logger, "Error guardando el bloque en el archivo\n");
        return -1;
    }

    return 0;
}

int _seleccionarNodoLectura(Bloque * bloque, DescriptorNodo ** nodo){
    int cantidadNodos = list_size(nombreNodos);
    int copiasDesconectadas = 0;

    while(copiasDesconectadas < 2){
        DescriptorNodo * descriptorNodo = dictionary_get(nodos, list_get(nombreNodos, punteroNodo));

        punteroNodo = (punteroNodo + 1) % cantidadNodos;

        if(descriptorNodo == NULL){
            continue;
        }

        if(strcmp(bloque->copia0.nodo, descriptorNodo->nombreNodo) == 0){
            if(descriptorNodo->socket == -1 || bloque->copia0.numeroBloque == -1){
                copiasDesconectadas++;
            }else{
                *nodo = descriptorNodo;
                return bloque->copia0.numeroBloque;
            }
        }else if(strcmp(bloque->copia1.nodo, descriptorNodo->nombreNodo) == 0){
            if(descriptorNodo->socket == -1 || bloque->copia1.numeroBloque == -1){
                copiasDesconectadas++;
            }else{
                *nodo = descriptorNodo;
                return bloque->copia1.numeroBloque;
            }
        }
    }

    return -1;
}