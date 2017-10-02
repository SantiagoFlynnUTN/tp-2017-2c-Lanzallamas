#include "estructurasFileSystem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commons/string.h>
#include "configuracionFileSystem.h"

void _persistirNodo(char * key, void * value);
void _persistirBitMap(char * strBitMap, char * nombreNodo, int bloques);

// Declaro estas variables globales para acumular durante la iteracion del diccionario
int bloquesTotales;
int bloquesLibres;
t_config * tablaNodos;
char * nombres;

void agregarNodoEnTabla(DescriptorNodo * newNodo){
    DescriptorNodo * descriptorNodo = (DescriptorNodo *) dictionary_get(nodos, newNodo->nombreNodo);

    if(descriptorNodo == NULL){
        crearBitMap(newNodo);

        dictionary_remove_and_destroy(nodos, newNodo->nombreNodo, free); // evito duplicados
        dictionary_put(nodos, newNodo->nombreNodo, newNodo);
    }else {
        descriptorNodo->socket = newNodo->socket;
        strcpy(descriptorNodo->ip, newNodo->ip);
        descriptorNodo->puerto = newNodo->puerto;

        dictionary_remove(nodos, descriptorNodo->nombreNodo);
        dictionary_put(nodos, descriptorNodo->nombreNodo, descriptorNodo);
    }
}

void crearBitMap(DescriptorNodo * newNodo){
    // divido la cantidad de bloques por 8 para tener un bit por bloque; el + 7 permite que: 2 / 8 = 1
    int bytes = (newNodo->bloques + 7) / 8;

    char * bitArray = (char *) malloc(sizeof(char) * bytes);

    newNodo->bitmap = bitarray_create_with_mode(bitArray, bytes, LSB_FIRST);
}

void guardarTablaNodos(){
    char * archivo = config_get_string_value(config, PATH_TABLA_NODOS);

    FILE * archivoNodos = fopen(archivo, "w"); // hago esto para crear el archivo si no existe
    fclose(archivoNodos);

    int cantidadNodos = dictionary_size(nodos);
    /*
     * 100 para el nombre de cada nodo
     * 1 coma por cada nodo
     * 2 corchetes
     */
    int chars = 101 * cantidadNodos + 2;
    int bytes = sizeof(char) * chars;
    nombres = (char *) malloc(bytes);
    memset(nombres, 0, bytes);

    tablaNodos = config_create(archivo);
    bloquesTotales = 0;
    bloquesLibres = 0;
    dictionary_iterator(nodos,_persistirNodo);

    char strBloquesTotales[5];
    intToString(bloquesTotales, strBloquesTotales);

    config_set_value(tablaNodos, TAMANIO, strBloquesTotales);

    char strBloquesLibres[5];
    intToString(bloquesLibres, strBloquesLibres);

    config_set_value(tablaNodos, LIBRE, strBloquesLibres);

    strcat(nombres, "]");

    config_set_value(tablaNodos, NODOS, nombres);

    config_save_in_file(tablaNodos, archivo);
    config_destroy(tablaNodos);
    free(nombres);
}

void guardarTablaDirectorio(){
    FILE * archivoDirectorio = fopen(config_get_string_value(config, PATH_TABLA_DIRECTORIO), "wb");

    int i;

    for(i = 0; i < 100; ++i){ // recorro toda la tabla
        // directorio no existente lo saleo, continuo el for porque puede haber otros si el directorio fue borrado
        if(tabla_Directorios[i].nombre[0] == '\0') continue;

        fwrite(&tabla_Directorios[i], sizeof(tabla_Directorios[i]), 1, archivoDirectorio);
    }

    fclose(archivoDirectorio);
}

void _persistirNodo(char * key, void * value){
    DescriptorNodo * nodo = (DescriptorNodo *)value;

    if(bloquesTotales){
        string_append_with_format(&nombres, ",%s", nodo->nombreNodo);
    }else{
        string_append_with_format(&nombres, "[%s", nodo->nombreNodo);
    }

    bloquesTotales += nodo->bloques;

    int bitmapSize = nodo->bloques;
    char strBitMap[bitmapSize + 1];
    memset(strBitMap, 0, bitmapSize + 1);

    while(bitmapSize--){
        if(bitarray_test_bit(nodo->bitmap, bitmapSize) == 0){ // bloque libre
            strBitMap[bitmapSize] = '0';
        }else{
            strBitMap[bitmapSize] = '1';
        }
    }

    bloquesLibres += nodo->bloquesLibres;

    char totalNodoKey[105];

    getKeyTotalBloquesNodo(totalNodoKey, nodo->nombreNodo);

    char strBloquesNodo[5];
    intToString(nodo->bloques, strBloquesNodo);

    config_set_value(tablaNodos, totalNodoKey, strBloquesNodo);

    char libreNodoKey[105];

    getKeyBloquesLibresNodo(libreNodoKey, nodo->nombreNodo);

    char strBloquesLibresNodo[5];
    intToString(nodo->bloquesLibres, strBloquesLibresNodo);

    config_set_value(tablaNodos, libreNodoKey, strBloquesLibresNodo);

    _persistirBitMap(strBitMap, nodo->nombreNodo, nodo->bloques);
}

void _persistirBitMap(char * strBitMap, char * nombreNodo, int bloques){
    char * archivo = (char *) malloc(sizeof(char) * 255);
    sprintf(archivo, config_get_string_value(config, PATH_BITMAPS), nombreNodo);

    FILE * f = fopen(archivo, "w");

    fwrite(strBitMap, bloques, 1, f);
    fclose(f);
    free(archivo);
}