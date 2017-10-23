/*
 * RGWorker.c
 *
 *  Created on: 22/10/2017
 *      Author: utnso
 */

#include <protocoloComunicacion.h>
#include <stdint.h>
#include "cliente.h"
#include <sockets.h>
#include <stdio.h>
#include "RGWorker.h"



void rutinaNoEncargado(encargadofd){
	NodoGlobal yo;
	zrecv(encargadofd, &yo, sizeof(yo), 0);


	printf("termine\n");
	//hago mi magia y muero (le envio el archivo)
}

void iniciarGlobal(int mastersock){
	int cantNodos;
	zrecv(mastersock, &cantNodos, sizeof(int), 0);

	NodoGlobal nodos[cantNodos];
	int socket_nodos[cantNodos];


	int i, pedArchivo;
	pedArchivo = SOLICITUDARCHIVOWORKER;
	for(i=0; i < cantNodos; i++){
		zrecv(mastersock, &nodos[i], sizeof(NodoGlobal), 0);
		conectarANodo(&socket_nodos[i], nodos[i].ip, nodos[i].puerto);
		zsend(socket_nodos[i], &pedArchivo, sizeof(int), 0);
		zsend(socket_nodos[i], &nodos[i], sizeof(NodoGlobal), 0);
		//aca recibiria el archivo
		close(socket_nodos[i]);
	}
	int ok = 1;
	zsend(mastersock, &ok, sizeof(int), 0);
	close(mastersock);
	printf("termino todo ok\n");
}
