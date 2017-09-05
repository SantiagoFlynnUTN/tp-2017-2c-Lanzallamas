#include "yama.h"

#ifndef YAMAOPERATIONS_H_
#define YAMAOPERATIONS_H_

#include "node.h"
#include "block.h"

typedef struct t_OperacionTransformacion{
	DescriptorNodo descriptor;
	DescriptorBloque dataBlock;
	char * archivoTemporal;
} OperacionTransformacion;

typedef struct t_OperacionReduccionLocal{
	DescriptorNodo descriptor;
	char * archivosTemporales; //ver como poner esto es un array de punteros
	char * archivoReducido;
} OperacionReduccionLocal;

typedef struct t_OperacionReduccionGlobal{
	DescriptorNodo descriptor;
	char * archivosTemporales; //ver como poner esto es un array de punteros
	char * archivoReducido;
} OperacionReduccionGlobal;

#endif /* YAMAOPERATIONS_H_ */