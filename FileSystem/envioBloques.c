#include "fileSystem.h"
#include "envioBloques.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "conexionesFileSystem.h"
#include "utilidadesFileSystem.h"

int _controlarEspacioDisponible(int cantidadBloques);
void _contarBloquesLibres(char * key, void * nodo);
int _enviarBloque(char * bloque, Ubicacion ubicacion);
Bloque * _crearBloque(int numeroBloque, long bytes);
void _calcularUbicacionBloque(Bloque * bloque);
int _obtenerNumeroBloqueLibre(t_bitarray * bitmap);
int _manejarEnvioBloque(char * bloque, Bloque * descriptorBloque);
void _destruirArchivo(Archivo * descriptorArchivo);
void _liberarBloque(Ubicacion bloque);

int bloquesLibres = 0;

int enviarBloques(char * archivo, char * archivoYamaFS){
    FILE * file = fopen(archivo, "r");

    if(file == NULL){ // no se encontró el archivo
        log_error(logger, "El archivo %s no existe\n", archivo);
        return -1;
    }

    int directorioPadre = calcularDirectorioPadre(archivoYamaFS);

    if(directorioPadre < 0){
        log_error(logger, "La ruta %s es inválida\n", archivoYamaFS);
        return -1;
    }

    struct stat fileStats;

    if(stat(archivo, &fileStats) != 0){ // error
        log_error(logger, "El archivo %s no existe\n", archivo);
        return -1;
    }

    int cantidadBloques = fileStats.st_size / MB + 1;

    if(!_controlarEspacioDisponible(cantidadBloques)){ // no hay espacio
        log_error(logger, "No hay espacio disponible para guardar el archivo; El archivo requiere %d bloques para cada copia; %d archivos en total\n", cantidadBloques, 2 * cantidadBloques);
        return -1;
    }

    char bloque[MB];

    memset(bloque, 0, MB); // limpio el bloque
    long ocupados = 0;

    ssize_t bytesLeidos;
    char * linea;
    size_t len;
    int numeroBloque = 0;

    Archivo * descriptorArchivo = (Archivo *)malloc(sizeof(*descriptorArchivo));
    strcpy(descriptorArchivo->ruta, archivoYamaFS);
    descriptorArchivo->tipo = TEXTO;
    descriptorArchivo->tamanio = fileStats.st_size;
    descriptorArchivo->directorioPadre = directorioPadre;
    descriptorArchivo->bloques = list_create();

    while((bytesLeidos = getline(&linea, &len, file)) > -1) { // hasta llegar al final del archivo
        if (ocupados + bytesLeidos > MB) { // no alcanza el bloque para guardar el renglón
            Bloque * descriptorBloque = _crearBloque(numeroBloque, ocupados);

            ocupados = 0;

            if(_manejarEnvioBloque(bloque, descriptorBloque) == -1){
                _destruirArchivo(descriptorArchivo);
                log_error(logger, "Error guardando los bloques en los datanode\n", archivo);
                return -1;
            }

            list_add(descriptorArchivo->bloques, descriptorBloque);
            memset(bloque, 0, MB);
            numeroBloque++;
        }

        strcat(bloque + ocupados, linea);
        ocupados += bytesLeidos;
    }

    if(ocupados != 0){ // quedan bytes para mandar para mandar
        Bloque * descriptorBloque = _crearBloque(numeroBloque, ocupados);

        if(_manejarEnvioBloque(bloque, descriptorBloque) == -1){
            _destruirArchivo(descriptorArchivo);
            log_error(logger, "Error guardando los bloques en los datanode\n", archivo);
            return -1;
        }

        list_add(descriptorArchivo->bloques, descriptorBloque);
    }

    fclose(file);

    registrarArchivo(descriptorArchivo);

    return 0;
}

int _controlarEspacioDisponible(int cantidadBloques){
    bloquesLibres = 0;
    dictionary_iterator(nodos,_contarBloquesLibres);

    return 2 * cantidadBloques <= bloquesLibres;
}

void _contarBloquesLibres(char * key, void * nodo){
    DescriptorNodo * descriptorNodo = (DescriptorNodo *) nodo;

    if(descriptorNodo->socket == -1){ // nodo desconectado no lo incluyo en los bloques disponibles
        return;
    }

    bloquesLibres += descriptorNodo->bloquesLibres;
}

int _enviarBloque(char * bloque, Ubicacion ubicacion){
    int mensaje = SETBLOQUE;
    DescriptorNodo * nodo = (DescriptorNodo *) dictionary_get(nodos, ubicacion.nodo);

    if (send(nodo->socket, &mensaje, sizeof(mensaje), 0) == -1 ||
        send(nodo->socket, &ubicacion.numeroBloque, sizeof(ubicacion.numeroBloque), 0) == -1 ||
        send(nodo->socket, bloque, sizeof(char) * MB, 0) ==-1){
        log_error(logger, "No puedo enviar el bloque al nodo %s\n", nodo->nombreNodo);
        return -1;
    }

    return 0;
}

Bloque * _crearBloque(int numeroBloque, long bytes){
    Bloque * bloque = (Bloque *) malloc(sizeof(*bloque));
    bloque->descriptor.numeroBloque = numeroBloque;
    bloque->descriptor.bytes = bytes;

    _calcularUbicacionBloque(bloque);

    return bloque;
}

void _calcularUbicacionBloque(Bloque * bloque){
    int cantidadNodos = list_size(nombreNodos);
    int copiasUbicadas = 0;
    int i = 0;

    while(copiasUbicadas < 2){
        DescriptorNodo * descriptorNodo = (DescriptorNodo *)dictionary_get(nodos, (char *)list_get(nombreNodos, i));

        if(descriptorNodo->bloquesLibres > 0 && descriptorNodo->socket > -1){
            if(copiasUbicadas == 0){
                strcpy(bloque->copia0.nodo, descriptorNodo->nombreNodo);
                bloque->copia0.numeroBloque = _obtenerNumeroBloqueLibre(descriptorNodo->bitmap);
            }else{
                strcpy(bloque->copia1.nodo, descriptorNodo->nombreNodo);
                bloque->copia1.numeroBloque = _obtenerNumeroBloqueLibre(descriptorNodo->bitmap);
            }

            descriptorNodo->bloquesLibres--;
            copiasUbicadas++;
        }

        i = (i + 1) % cantidadNodos;
    }
}

int _obtenerNumeroBloqueLibre(t_bitarray * bitmap) {
    int i = 0,
            numeroBloque = -1;

    while (numeroBloque == -1) {
        if (bitarray_test_bit(bitmap, i) == 0) {
            bitarray_set_bit(bitmap, i);
            numeroBloque = i;
        }
        i++;
    }

    return numeroBloque;
}

int _manejarEnvioBloque(char * bloque, Bloque * descriptorBloque){
    int resultado;
    if(_enviarBloque(bloque, descriptorBloque->copia0) != 0 ||
       _enviarBloque(bloque, descriptorBloque->copia1) != 0){
        return -1;
    }

    DescriptorNodo * nodo = (DescriptorNodo *) dictionary_get(nodos, descriptorBloque->copia0.nodo);
    if(recv(nodo->socket, &resultado, sizeof(resultado), 0) <= 0 || !resultado){
        return -1;
    }

    nodo = (DescriptorNodo *) dictionary_get(nodos, descriptorBloque->copia1.nodo);
    if(recv(nodo->socket, &resultado, sizeof(resultado), 0) <= 0 || !resultado){
        return -1;
    }

    return 0;
}

void _destruirArchivo(Archivo * descriptorArchivo){
    int bloques = list_size(descriptorArchivo->bloques);
    int i;

    for(i = 0; i < bloques; ++i){
        Bloque * descriptorBloque = (Bloque *)list_get(descriptorArchivo->bloques, i);
        list_remove(descriptorArchivo->bloques, i);

        _liberarBloque(descriptorBloque->copia0);
        _liberarBloque(descriptorBloque->copia1);

        free(descriptorBloque);
    }

    list_destroy(descriptorArchivo->bloques);

    free(descriptorArchivo);
}

void _liberarBloque(Ubicacion bloque){
    DescriptorNodo * nodo = (DescriptorNodo *) dictionary_get(nodos, bloque.nodo);
    bitarray_clean_bit(nodo->bitmap, bloque.numeroBloque);

    nodo->bloquesLibres++;
}