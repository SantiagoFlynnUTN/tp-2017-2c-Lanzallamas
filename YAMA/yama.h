#include <commons/collections/dictionary.h>
#include <commons/config.h>
#include <commons/log.h>
#include <stdint.h>

#ifndef YAMA_H_
#define YAMA_H_

/* ENUMS */
typedef enum{TRANSFORMACION=1, REDUCCIONLOCAL, REDUCCIONGLOBAL} TipoOperacion;
typedef enum{ENPROCESO=1, ERROR, FINALIZADO} Estado;
typedef enum{ROUNDROBIN=1, WEIGHTEDROUNDROBIN} AlgoritmoBalanceo;

/* ESTRUCTURAS */
typedef struct t_TablaEstado{
	int jobId;
	int masterId;
	char * nodoId;
	int numeroBloque;
	TipoOperacion etapa;
	char * archivoTemporal;
	Estado estado;
} TablaEstado;

// Está repetido sacar de acá cuando hagamos una librería común
typedef struct t_DescriptorNodo{
	char * nodoId;
	char * ip;
	int puerto;
} DescriptorNodo;

// Está repetido sacar de acá cuando hagamos una librería común
typedef struct t_DescriptorBloque{
	int numeroBloque;
	long bytes;
} DescriptorBloque;

typedef struct t_Worker{
	DescriptorNodo descriptor;
	int tareasRealizadas; // agregué estos campos porque me pareció que está bueno tenerlos disponibles 
	int tareasEnProceso; // sino el load balancer tiene que recorrer la tabla cada vez
} Worker;

typedef struct {
	char ip[20];
	uint16_t puerto;
} ConexionFileSystem;

/* VARIABLES GLOBALES Y DEFINES*/
#define ARCHIVO_CONFIGURACION "yama.conf"
#define ARCHIVO_LOGGER "yama.log"
#define MODULO "YAMA"
#define FS_IP "FS_IP"
#define FS_PUERTO "FS_PUERTO"
#define RETARDO_PLANIFICACION "RETARDO_PLANIFICACION"
#define ALGORITMO_BALANCEO "ALGORITMO_BALANCEO"
#define ROUND_ROBIN "ROUND_ROBIN"
#define WEIGHTED_ROUND_ROBIN "WEIGHTED_ROUND_ROBIN"

t_config * config;
t_log * logger;
ConexionFileSystem conexionFileSystem;
AlgoritmoBalanceo algoritmoBalanceo;
int retardoPlanificacion;



/* FUNCIONES */
void inicializarYAMA();
void recargarConfiguracion();

#endif /* YAMA_H_ */
