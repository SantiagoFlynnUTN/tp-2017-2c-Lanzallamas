#include "yama.h"

#ifndef TP_2017_2C_LANZALLAMAS_PLANIFICACIONYAMA_H
#define TP_2017_2C_LANZALLAMAS_PLANIFICACIONYAMA_H

void planificacion(nodos, DescriptorBloque *bloques);
int estaEnNodo(DescriptorBloque bloque, InfoNodo *pClock);
void setAvailability(nodos);
void wL(InfoNodo nodo);
int pWl(InfoNodo nodo);
void setTareasRealizadas(InfoNodo nodos);
void sortByAvailability(nodos);
void sortByTareasRealizadas(nodos);


#endif //TP_2017_2C_LANZALLAMAS_PLANIFICACIONYAMA_H
