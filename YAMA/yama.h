typedef enum{TRANSFORMACION, REDUCCIONLOCAL, REDUCCIONGLOBAL} TipoOperacion;
typedef enum{ENPROCESO, ERROR, FINALIZADO} Estado;
typedef enum{ROUNDROBIN, WEIGHTEDROUNDROBIN} AlgoritmoBalanceo;

typedef struct t_TablaEstado{
	int jobId;
	int masterId;
	int nodoId;
	int numeroBloque;
	TipoOperacion etapa;
	char * archivoTemporal;
	Estado estado;
} TablaEstado;

// Está repetido sacar de acá cuando hagamos una librería común
typedef struct t_DescriptorNodo{
	int nodeId;
	char * ip;
	int puerto;
} DescriptorNodo;

typedef struct t_Worker{
	DescriptorNodo descriptor;
	int tareasRealizadas; // agregué estos campos porque me pareció que está bueno tenerlos disponibles 
	int tareasEnProceso; // sino el load balancer tiene que recorrer la tabla cada vez
} Worker;