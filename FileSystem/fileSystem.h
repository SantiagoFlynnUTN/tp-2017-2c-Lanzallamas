#import stdbool.h

typedef enum{TEXTO, BINARIO} TipoArchivo;

typedef struct t_Directorio{
	int id;
	char nombre[255];	
	int padre;
} Directorio;

typedef struct t_DescriptorNodo{
	int nodeId;
	char * ip;
	int puerto;
} DescriptorNodo;

// Falta el bitmap, no se cuál es la mejor
typedef struct t_DataNode{
	DescriptorNodo descriptor;
	int bloquesTotales;
	int bloquesLibres;
};

typedef struct t_Ubicacion{
	int nodeId;
	int numeroBloque;
} Ubicacion;

typedef struct t_Bloque{
	DescriptorBloque descriptor;
	Ubicacion copia0;
	Ubicacion copia1;
	long bytes;
	char * ruta;
	bool esDirectorio;
	int directorioPadre;
	long long MD5; // long long es de al menos 64 bits, el MD5 tiene que ser de 128 bits. hay que buscar un int 128
} Bloque;

typedef struct t_Archivo{
	long bytes;
	TipoArchivo tipo;
	Bloque * bloques;
} Archivo;



