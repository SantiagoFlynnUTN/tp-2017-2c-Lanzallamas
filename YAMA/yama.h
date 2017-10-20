#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <stdint.h>

#ifndef YAMA_H_
#define YAMA_H_

/* ENUMS */
typedef enum{TRANSFORMACION=1, REDUCCIONLOCAL} TipoOperacion;
typedef enum{ENPROCESO=1, ERRORYAMA, FINALIZADO} Estado;
typedef enum{CLOCK=1, WEIGHTEDCLOCK} AlgoritmoBalanceo;

/* ESTRUCTURAS */

typedef struct{
	int tipomensaje;
	char ruta[255];
} __attribute__((packed)) SolicitudFS;

typedef struct t_TablaEstado{
	int jobId;
	int masterId;
	char nombreNodo[100];
	int numeroBloque;
	TipoOperacion etapa;
	char archivoTemporal[255];
	Estado estado;
} EntradaTablaEstado;

typedef struct{
	char ruta[255];
}rutaArchivo;

typedef struct{
	char nombreNodo[100];
	char ip[20];
	uint16_t puerto;
	int cantidadTemporales;
	char archivoReducido[255];
} __attribute__((packed)) operacionReduccion;

typedef struct{
	char nombreNodo[100];
	char ip[20];
	uint16_t puerto;
	int bloque;
	int bytes;
	char ruta[255];
} __attribute__((packed)) operacionTransformacion;

// Está repetido sacar de acá cuando hagamos una librería común
typedef struct t_DescriptorBloque{
	int numeroBloque;
	long bytes;
	char copia0;
	char copia1;
	char nodoAsignado;
	DescriptorBloque *siguiente;
} DescriptorBloque;

typedef struct t_Worker{
	InfoNodo descriptor;
	int tareasRealizadas; // agregué estos campos porque me pareció que está bueno tenerlos disponibles 
	int tareasEnProceso; // sino el load balancer tiene que recorrer la tabla cada vez
} Worker;

typedef struct {
	char ip[20];
	uint16_t puerto;
} ConexionFileSystem;

typedef struct {
	char nombre[100];
	int bloque;
	char ip[20];
	uint16_t puerto;
	int disponibilidad;
	int trabajoActual;
	int tareasHistoricas;
	InfoNodo *siguiente;
} InfoNodo;

typedef struct {
	InfoNodo primero;
};

/* VARIABLES GLOBALES Y DEFINES*/
#define ARCHIVO_CONFIGURACION "yama.conf"
#define ARCHIVO_LOGGER "yama.log"
#define MODULO "YAMA"
#define FS_IP "FS_IP"
#define FS_PUERTO "FS_PUERTO"
#define RETARDO_PLANIFICACION "RETARDO_PLANIFICACION"
#define ALGORITMO_BALANCEO "ALGORITMO_BALANCEO"
#define CLOCK "CLOCK"
#define WEIGHTED_CLOCK "WEIGHTED_CLOCK"
#define DISPONIBILIDAD_BASE "DISPONIBILIDAD_BASE"

t_config * config;
t_log * logger;
ConexionFileSystem conexionFileSystem;
AlgoritmoBalanceo algoritmoBalanceo;
int retardoPlanificacion;
int sock_fs;
t_list* tablaEstado;
int disponibilidad_base;
unsigned int wlMax;
unsigned int wl;
InfoNodo pClock;
InfoNodo auxiliar;

/* FUNCIONES */
void inicializarYAMA();
void recargarConfiguracion();

#endif /* YAMA_H_ */
