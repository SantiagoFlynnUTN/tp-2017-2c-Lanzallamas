#include "yama.h"

#ifndef TP_2017_2C_LANZALLAMAS_PLANIFICACIONYAMA_H
#define TP_2017_2C_LANZALLAMAS_PLANIFICACIONYAMA_H

void ordenarNodos(t_list * nodos);
int estaEnNodo(DescriptorBloque bloque, InfoNodo *pClock);
void planificarBloquesYEnviarAMaster(int socket_master, int bloques, t_list * listaNodos);
void generarArchivoTemporal(char * nombre, char * file);


#endif //TP_2017_2C_LANZALLAMAS_PLANIFICACIONYAMA_H
