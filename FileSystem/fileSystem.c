#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "fileSystem.h"
#include "estructurasFileSystem.h"
#include "configuracionFileSystem.h"

void _cargarConfiguracion();
void _crearLogger();
void _logConfig();
void _cargarFileSystem();
void _cargarTablaNodos(t_config * tablaNodos);
int _cargarBitMap(DescriptorNodo * nodo);

void printNodo(DescriptorNodo * a){
    log_info(logger, "Nodo Conectado\nNombre: %s\nDireccion Worker: %s:%u\nCantidad de bloques: %d\n",
             a->nombreNodo, a->ip, a->puerto, a->bloques);
}

void asociarNodo(int socket){
    DescriptorNodo * newNodo = (DescriptorNodo *)malloc(sizeof(*newNodo));
    recv(socket, newNodo->nombreNodo, 100 * sizeof(char), 0);
    recv(socket, newNodo->ip, 20 * sizeof(char), 0);
    recv(socket, &newNodo->puerto, sizeof(newNodo->puerto), 0);
    recv(socket, &newNodo->bloques, sizeof(newNodo->bloques), 0);

    crearBitMap(newNodo);
    agregarNodoEnTabla(newNodo);

    printNodo(dictionary_get(nodos, newNodo->nombreNodo));
    guardarTablaNodos(); //TODO remove
}

void _cargarConfiguracion(){
    config = config_create(ARCHIVO_CONFIGURACION);

    if(config == NULL){
        log_error(logger, "El archivo de configuración %s no pudo ser encontrado\n", ARCHIVO_CONFIGURACION);
        exit(-1);
    }

    if(!config_has_property(config, PUERTO_FILESYSTEM) ||
       !config_has_property(config, PATH_TABLA_NODOS)  ||
       !config_has_property(config, PATH_BITMAPS)){
        log_error(logger, "El archivo de configuración %s no tiene los campos necesarios\n", ARCHIVO_CONFIGURACION);
        exit(-2);
    }
}

void inicializarFileSystem(){
    nodos = dictionary_create();
    _crearLogger();
    _cargarConfiguracion();
    _cargarFileSystem();
    _logConfig();
}

void _cargarFileSystem(){
    t_config * tablaNodos = config_create(config_get_string_value(config, PATH_TABLA_NODOS));

    if(tablaNodos == NULL){
        log_info(logger, "No hay estado previo\n");
        return;
    }

    _cargarTablaNodos(tablaNodos);
    config_destroy(tablaNodos);
}

void _cargarTablaNodos(t_config * tablaNodos){
    char ** nombresNodos = config_get_array_value(tablaNodos, NODOS);

    while(*nombresNodos){
        DescriptorNodo * newNodo = (DescriptorNodo *)malloc(sizeof(*newNodo));
        strcpy(newNodo->nombreNodo, *nombresNodos);
        // FALTA IP Y PUERTO y bitmap
        char totalNodoKey[105];
        getKeyTotalBloquesNodo(totalNodoKey, *nombresNodos);

        newNodo->bloques = config_get_int_value(tablaNodos, totalNodoKey);

        if(_cargarBitMap(newNodo) == 0){
            log_info(logger, "%s\nBloques:%d", *nombresNodos, newNodo->bloques);
            agregarNodoEnTabla(newNodo);
        }else{
            log_info(logger, "%s fue descartado por no tener bitmap", *nombresNodos);
            free(newNodo);
        }

        nombresNodos++;
    }
}

int _cargarBitMap(DescriptorNodo * nodo){
    char * archivo = (char *) malloc(sizeof(char) * 255);
    sprintf(archivo, config_get_string_value(config, PATH_BITMAPS), nodo->nombreNodo);

    FILE * f = fopen(archivo, "r");

    if(f == NULL){
        return 1;
    }

    char strBitMap[nodo->bloques + 1];

    fread(strBitMap, nodo->bloques, 1, f);

    fclose(f);

    crearBitMap(nodo);

    int i;

    for(i = 0; i < nodo->bloques; ++i){
        if(strBitMap[i] == '0'){
            bitarray_clean_bit(nodo->bitmap, i);
        }else{
            bitarray_set_bit(nodo->bitmap, i);
        }
    }

    free(archivo);
    return 0;
}

void _crearLogger(){
    logger = log_create(ARCHIVO_LOGGER, MODULO, true, LOG_LEVEL_INFO);
}

void _logConfig(){
    log_debug(logger, "Config:\nPUERTO_FILESYSTEM: %d\nPATH_TABLA_NODOS: %s\nPATH_BITMAPS: %s",
              config_get_int_value(config, PUERTO_FILESYSTEM),
              config_get_string_value(config, PATH_TABLA_NODOS),
              config_get_string_value(config, PATH_BITMAPS));
}