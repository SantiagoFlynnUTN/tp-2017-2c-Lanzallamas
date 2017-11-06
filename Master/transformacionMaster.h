#ifndef TRANSFORMACIONMASTER_H_
#define TRANSFORMACIONMASTER_H_
#include <commons/collections/list.h>

typedef struct{
	long cantidadBytes;
	int bloque;
	char nombreTemp[255];
}__attribute__((packed))
mensajeTransformacion;

int respuestaSolicitud(int socket_yama);
void mandarTransformacionNodo(int socket_nodo, int socket_yama, int cantidadWorkers, t_list* tiemposTransformacion);
void atenderConexion(int socket);

#endif /* TRANSFORMACIONMASTER_H_ */
