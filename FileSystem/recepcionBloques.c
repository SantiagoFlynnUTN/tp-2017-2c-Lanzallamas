#include "recepcionBloques.h"
#include "envioBloques.h"
#include <sys/socket.h>
#include "conexionesFileSystem.h"

int _guardarBloque(DescriptorNodo * nodo, int bloque, long bytes, FILE * archivo);


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

        DescriptorNodo * descriptorNodo = (DescriptorNodo *) dictionary_get(nodos, bloque->copia0.nodo);

        if(descriptorNodo->socket != -1){
            int bloqueGuardado = _guardarBloque(descriptorNodo, bloque->copia0.numeroBloque, bloque->descriptor.bytes, archivoFinal);

            if(bloqueGuardado != 0){
                fclose(archivoFinal);
                remove(rutaArchivoFinal);
                return -1;
            }

        } else{
            descriptorNodo = (DescriptorNodo *) dictionary_get(nodos, bloque->copia1.nodo);

            if(descriptorNodo->socket != -1){
                int bloqueGuardado = _guardarBloque(descriptorNodo, bloque->copia1.numeroBloque, bloque->descriptor.bytes, archivoFinal);

                if(bloqueGuardado != 0){
                    fclose(archivoFinal);
                    remove(rutaArchivoFinal);
                    return -1;
                }
            } else{
                fclose(archivoFinal);
                remove(rutaArchivoFinal);
                log_error(logger, "El bloque %d del archivo %s no tiene copias disponibles\n", i, rutaArchivoFS);
                return -1;
            }
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