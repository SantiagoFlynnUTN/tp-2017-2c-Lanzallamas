#include "yama.h"
#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_


enum tipoProceso {YAMA, FILESYSTEM, DATANODE, MASTER, WORKER};

void manejarDatos(int buf, int socket);
void enviarSolicitudReduccion(int socket, t_list * transformacionesRealizadas);

#endif /* SERIALIZACION_H_ */
