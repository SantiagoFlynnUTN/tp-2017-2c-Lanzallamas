#include "yama.h"

#ifndef TP_2017_2C_LANZALLAMAS_PLANIFICACIONYAMA_H
#define TP_2017_2C_LANZALLAMAS_PLANIFICACIONYAMA_H

void planificacion(t_list * nodos, int cantBloques, int socketMaster);
int estaEnNodo(DescriptorBloque bloque, InfoNodo *pClock);
void setAvailability(t_list * nodos);
void wL(InfoNodo * nodo);
int pWl(InfoNodo * nodo, int max);
void setTareasRealizadas(InfoNodo * nodos);
void sortByAvailability(t_list * nodos);
void sortByTareasRealizadas(t_list * nodos);
void sumarTrabajos(t_list * nodos);


#endif //TP_2017_2C_LANZALLAMAS_PLANIFICACIONYAMA_H
