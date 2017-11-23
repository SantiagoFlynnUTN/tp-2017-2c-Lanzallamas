/*
 * conexiones.c
 *
 *  Created on: 3/9/2017
 *      Author: utnso
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "utilidadesFileSystem.h"
#include "conexionesFileSystem.h"
#include "mainFS.h"

void _enviarInfoBloque(int socket, DescriptorNodo * nodo, int bloque);

//a desarrollar
void responderYAMA(int socketYAMA){
	char rutaArchivo[255];
	recv(socketYAMA, rutaArchivo, sizeof(char) * 255, MSG_WAITALL);
	Archivo* descriptorArchivo = (Archivo*) dictionary_get(archivos, rutaArchivo);
	if(descriptorArchivo == NULL || !archivoDisponible(descriptorArchivo)){
		int mensaje = -2;
		send(socketYAMA, &mensaje, sizeof(mensaje), 0);
		return;
	}

	int mensaje = 0;
	int bloques = list_size(descriptorArchivo->bloques);

	send(socketYAMA, &mensaje, sizeof(mensaje), 0);
	send(socketYAMA, &bloques, sizeof(bloques), 0);

	int i;

	for(i = 0; i < bloques; ++i){
		Bloque * bloque = (Bloque *) list_get(descriptorArchivo->bloques, i);
		send(socketYAMA, &bloque->descriptor.bytes, sizeof(bloque->descriptor.bytes), 0);
		DescriptorNodo * descriptorNodoCopia0 = (DescriptorNodo *) dictionary_get(nodos, bloque->copia0.nodo);
		DescriptorNodo * descriptorNodoCopia1 = (DescriptorNodo *) dictionary_get(nodos, bloque->copia1.nodo);

		if(descriptorNodoCopia0->socket == -1){
			int cantidad = 1;
			send(socketYAMA, &cantidad, sizeof(cantidad), 0);
			_enviarInfoBloque(socketYAMA, descriptorNodoCopia1, bloque->copia1.numeroBloque);
		}else if(descriptorNodoCopia1->socket == -1){
			int cantidad = 1;
			send(socketYAMA, &cantidad, sizeof(cantidad), 0);
			_enviarInfoBloque(socketYAMA, descriptorNodoCopia0, bloque->copia0.numeroBloque);
		}else{
			int cantidad = 2;
			send(socketYAMA, &cantidad, sizeof(cantidad), 0);
			_enviarInfoBloque(socketYAMA, descriptorNodoCopia0, bloque->copia0.numeroBloque);
			_enviarInfoBloque(socketYAMA, descriptorNodoCopia1, bloque->copia1.numeroBloque);
		}
	}
}

void _enviarInfoBloque(int socket, DescriptorNodo * nodo, int bloque){
	send(socket, nodo->nombreNodo, sizeof(char) * 100, 0);
	send(socket, nodo->ip, sizeof(char) * 20, 0);
	send(socket, &nodo->puerto, sizeof(nodo->puerto), 0);
	send(socket, &bloque, sizeof(bloque), 0);
}
