/*
 * fileSystem.c
 *
 *  Created on: 9/9/2017
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "fileSystem.h"


void _cargarConfiguracion();
void _crearLogger();

void printNodo(DescriptorNodo * a){
    log_info(logger, "Nodo Conectado\nNombre: %s\nDireccion Worker: %s:%u\nCantidad de bloques: %d\n",
             a->nombreNodo, a->ip, a->puerto, a->bloques);
}

void asociarNodo(int socket){
    DescriptorNodo newNodo;
    recv(socket, newNodo.nombreNodo, 100 * sizeof(char), 0);
    recv(socket, newNodo.ip, 20 * sizeof(char), 0);
    recv(socket, &newNodo.puerto, sizeof(newNodo.puerto), 0);
    recv(socket, &newNodo.bloques, sizeof(newNodo.bloques), 0);
    // divido la cantidad de bloques por 8 para tener un bit por bloque; el + 7 permite que: 2 / 8 = 1
    int bytes = (newNodo.bloques + 7) / 8;

    char * bitArray = (char *) malloc(sizeof(char) * bytes);

    newNodo.bitmap = bitarray_create_with_mode(bitArray, bytes, LSB_FIRST);

    dictionary_put(nodos, newNodo.nombreNodo, &newNodo);
    printNodo(dictionary_get(nodos, newNodo.nombreNodo));
}

void _cargarConfiguracion(){
}

void inicializarFileSystem(){
    nodos = dictionary_create();
    _crearLogger();
}

void _crearLogger(){
    logger = log_create(ARCHIVO_LOGGER, MODULO, true, LOG_LEVEL_INFO);
}