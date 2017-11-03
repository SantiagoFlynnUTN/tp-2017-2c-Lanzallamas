#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "fileSystem.h"
#include "estructurasFileSystem.h"
#include "configuracionFileSystem.h"
#include "inicializacionFileSystem.h"
#include "funcionesConsolaFS.h"
#include "envioBloques.h"

void _cargarConfiguracion();
void _crearLogger();
void _logConfig();
void _cargarFileSystem();

void inicializarFileSystem(){
    nodos = dictionary_create();
    nombreNodos = list_create();
    archivos = dictionary_create();
    _crearLogger();
    _cargarConfiguracion();
    _cargarFileSystem();
    _logConfig();
}

void persistirFileSystem(){
    guardarTablaNodos();
    guardarTablaDirectorio();
    guardarArchivos();
}

void printNodo(DescriptorNodo * nodo){
    log_info(logger, "Nodo Conectado en socket: %d\nNombre: %s\nDireccion Worker: %s:%u\nCantidad de bloques: %d\nCantidad de bloques libres: %d\n",
              nodo->socket, nodo->nombreNodo, nodo->ip, nodo->puerto, nodo->bloques, nodo->bloquesLibres);
}

void asociarNodo(int socket){
    DescriptorNodo * newNodo = (DescriptorNodo *)malloc(sizeof(*newNodo));
    newNodo->socket = socket;
    recv(socket, newNodo->nombreNodo, 100 * sizeof(char), 0);
    recv(socket, newNodo->ip, 20 * sizeof(char), 0);
    recv(socket, &newNodo->puerto, sizeof(newNodo->puerto), 0);
    recv(socket, &newNodo->bloques, sizeof(newNodo->bloques), 0);

    newNodo->bloquesLibres = newNodo->bloques;

    agregarNodoEnTabla(newNodo);

    printNodo(dictionary_get(nodos, newNodo->nombreNodo));
}

void _cargarConfiguracion(){
    config = config_create(ARCHIVO_CONFIGURACION);

    if(config == NULL){
        log_error(logger, "El archivo de configuración %s no pudo ser encontrado\n", ARCHIVO_CONFIGURACION);
        exit(-1);
    }

    if(!config_has_property(config, PUERTO_FILESYSTEM) ||
       !config_has_property(config, PATH_TABLA_NODOS)  ||
       !config_has_property(config, PATH_BITMAPS) ||
       !config_has_property(config, PATH_TABLA_DIRECTORIO) ||
       !config_has_property(config,PATH_DIR_ARCHIVOS) ||
       !config_has_property(config, PATH_DIR_ARCHIVOS_FORMAT)){
        log_error(logger, "El archivo de configuración %s no tiene los campos necesarios\n", ARCHIVO_CONFIGURACION);
        exit(-2);
    }
}

void _cargarFileSystem(){
    t_config * tablaNodos = config_create(config_get_string_value(config, PATH_TABLA_NODOS));

    if(cargarTablaDirectorio() != 0){ // no hay directorios
        log_info(logger, "No hay directorios registrados\n");

        strcpy(tabla_Directorios[0].nombre, ROOT_DIR);
        tabla_Directorios[0].padre = -1;
        tabla_Directorios[0].id = 0;
        listaArchivosDirectorios[0] = list_create();
        return;
    }

    if(tablaNodos == NULL){
        log_info(logger, "No hay nodos registrados\n");
        return;
    }

    cargarTablaNodos(tablaNodos);

    if(cargarTablaArchivos() != 0){ // error levantando los archivos
        log_error(logger, "Error levantando la tabla de archivos, el file system se va a formatear");
        //formatFileSystem(); // limpio los nodos
    }

    config_destroy(tablaNodos);
}

void _crearLogger(){
    logger = log_create(ARCHIVO_LOGGER, MODULO, true, LOG_LEVEL_INFO);
}

void _logConfig(){
    log_debug(logger, "Config:\nPUERTO_FILESYSTEM: %d\nPATH_TABLA_NODOS: %s\nPATH_BITMAPS: %s\nPATH_TABLA_DIRECTORIO: %s\nPATH_DIR_ARCHIVOS: %s\n",
              config_get_int_value(config, PUERTO_FILESYSTEM),
              config_get_string_value(config, PATH_TABLA_NODOS),
              config_get_string_value(config, PATH_BITMAPS),
              config_get_string_value(config, PATH_TABLA_DIRECTORIO),
              config_get_string_value(config, PATH_DIR_ARCHIVOS),
              config_get_string_value(config, PATH_DIR_ARCHIVOS_FORMAT));
}