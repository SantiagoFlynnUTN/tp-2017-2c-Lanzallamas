#include <protocoloComunicacion.h>
#include <stdint.h>
#include "cliente.h"
#include <sockets.h>
#include <stdio.h>
#include "RGWorker.h"
#include "worker.h"

void rutinaNoEncargado(encargadofd){
	NodoGlobal yo;
	zrecv(encargadofd, &yo, sizeof(yo), 0);


	printf("termine\n");
	//hago mi magia y muero (le envio el archivo)
}

void iniciarGlobal(int mastersock){
	char archivo[255];
	int cantidad;
	int i;

	zrecv(mastersock, archivo, sizeof(char) * 255, 0);

	log_info(logger, "Archivo mío %s", archivo);

	zrecv(mastersock, &cantidad, sizeof(cantidad), 0);

	NodoGlobal nodos[cantidad];

	for(i = 0; i < cantidad; ++i){
		zrecv(mastersock, nodos[i].nombre, sizeof(char) * 100, 0);
		zrecv(mastersock, nodos[i].ip, sizeof(char) * 20, 0);
		zrecv(mastersock, &nodos[i].puerto, sizeof(nodos[i].puerto), 0);
		zrecv(mastersock, nodos[i].archivoReducido, sizeof(char) * 255, 0);

		printf("NODO:%s\nIP:%s\nPUERTO:%d\nARCHIVO:%s\nENCARGADO:NO\n", nodos[i].nombre, nodos[i].ip, nodos[i].puerto, nodos[i].archivoReducido);
	}

	if(cantidad == 0){
		int ok = 0;
		zsend(mastersock, &ok, sizeof(ok), 0);
	}






	/*int cantNodos;
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
	printf("termino todo ok\n");*/
}
