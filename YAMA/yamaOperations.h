#include "yama.h"

#ifndef YAMAOPERATIONS_H_
#define YAMAOPERATIONS_H_

typedef struct t_OperacionTransformacion{
	DescriptorNodo descriptor;
	DescriptorBloque dataBlock;
	char * archivoTemporal;
} OperacionTransformacion;

typedef struct t_OperacionReduccionLocal{
	DescriptorNodo descriptor;
	char ** archivosTemporales;
	char * archivoReducido;
} OperacionReduccionLocal;

typedef struct t_OperacionReduccionGlobal{
	DescriptorNodo descriptor;
	char ** archivosTemporales;
	char * archivoReducido;
} OperacionReduccionGlobal;

#endif /* YAMAOPERATIONS_H_ */
