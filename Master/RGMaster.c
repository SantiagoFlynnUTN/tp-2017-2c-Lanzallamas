/*
 * RGMaster.c
 *
 *  Created on: 22/10/2017
 *      Author: utnso
 */

#include <stdint.h>
#include <protocoloComunicacion.h>
#include <sockets.h>
#include "cliente.h"
#include "RGMaster.h"



void reduccionGlobal(int socket_yama){


	NodoEncargado encargado;
	NodoGlobal* nodos;
	int redGlo, cantNodos;
	redGlo = REDGLOBAL;
	int operacionGlobal = REDUCCIONGLOBAL;

	zsend(socket_yama, &redGlo, sizeof(int), 0);
	zrecv(socket_yama, &encargado, sizeof(encargado), 0);

	zrecv(socket_yama, &cantNodos, sizeof(int), 0);
	int socket_encargado;
	iniciarConexionANodo(&socket_encargado, encargado.ip, encargado.puerto);
	zsend(socket_encargado, &operacionGlobal, sizeof(int), 0);
	zsend(socket_encargado, &cantNodos, sizeof(int), 0);
	nodos = (NodoGlobal*)malloc(sizeof(NodoGlobal) * cantNodos);


	int i;
	for(i=0; i < cantNodos; i++){
		zrecv(socket_yama, &nodos[i], sizeof(NodoGlobal), 0);
		zsend(socket_encargado, &nodos[i], sizeof(NodoGlobal), 0);
	}
	int j = 0;
	zrecv(socket_encargado, &j, sizeof(int), 0);
	if(j) printf("el Job termino ok\n");

}
