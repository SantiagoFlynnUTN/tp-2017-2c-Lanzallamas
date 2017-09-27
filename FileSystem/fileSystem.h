#include <stdbool.h>
#include <stdint.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <commons/collections/dictionary.h>

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

/* ENUMS */
typedef enum{TEXTO, BINARIO} TipoArchivo;

/* ESTRUCTURAS */
typedef struct t_Directorio{
    int id;
    char nombre[255];
    int padre;
} Directorio;

typedef struct t_DescriptorNodo{
    char nombreNodo[100];
    char ip[20];
    uint16_t puerto;
    int bloques;
    t_bitarray * bitmap;
} DescriptorNodo;

// Falta el bitmap, no se cuál es la mejor
typedef struct t_DataNode{
    DescriptorNodo descriptor;
    int bloquesTotales;
    int bloquesLibres;
} DataNode;

typedef struct t_Ubicacion{
    int nodeId;
    int numeroBloque;
} Ubicacion;

typedef struct t_DescriptorBloque{
    int numeroBloque;
    long bytes;
} DescriptorBloque;

typedef struct t_Bloque{
    DescriptorBloque descriptor;
    Ubicacion copia0;
    Ubicacion copia1;
} Bloque;

typedef struct t_Archivo{
    TipoArchivo tipo;
    Bloque * bloques;
    char * ruta;
    bool esDirectorio;
    int directorioPadre;
    long long MD5; // long long es de al menos 64 bits, el MD5 tiene que ser de 128 bits. hay que buscar un int 128
} Archivo;

Directorio tabla_Directorios [100];

/* VARIABLES GLOBALES y DEFINES*/
#define ARCHIVO_LOGGER "fileSystem.log"
#define ARCHIVO_CONFIGURACION "fileSystem.conf"
#define MODULO "FILESYSTEM"
#define MAX_LENGTH 255
#define MAX_DIRECTORIOS 100
#define PUERTO_FILESYSTEM "PUERTO_FILESYSTEM"
#define PATH_TABLA_NODOS "PATH_TABLA_NODOS"
#define PATH_BITMAPS "PATH_BITMAPS"

t_dictionary* nodos;
t_log * logger;
t_config * config;

/* FUNCIONES */
void asociarNodo(int socket);
void inicializarFileSystem();


#endif /* FILESYSTEM_H_ */