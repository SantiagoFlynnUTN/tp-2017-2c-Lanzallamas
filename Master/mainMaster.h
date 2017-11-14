#include "master.h"
#include <pthread.h>

#ifndef MAIN_H_
#define MAIN_H_

int socket_yama;
pthread_mutex_t mutexTransformacion;
pthread_mutex_t mutexReduccion;
void nuevoCliente(int socket_cliente);
void atenderConexion(int socket);
void calcularMaximos();
double tiempoTotalJob;
double tiempoTotalTransf;
double tiempoTotalRedu;
int cantTransfActual;
int cantReduActual;
int transformacionesOk;
int reduccionesOk;
int fallosTransf;
int fallosRedu;
int cargaMaxima;
int maxT;
int maxR;

void calcularMaximos();

#endif /* MAIN_H_ */
