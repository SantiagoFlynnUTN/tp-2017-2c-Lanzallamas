#include <stdint.h>
#include <protocoloComunicacion.h>
#include <sockets.h>
#include "cliente.h"
#include "RGMaster.h"
#include "master.h"

void reduccionGlobal(int socket_yama){
	int cantidad;

	int i = 0;

	zrecv(socket_yama, &cantidad, sizeof(cantidad), 0);

	OperacionReduccionGlobal operacionReduccion[cantidad - 1];
	OperacionReduccionGlobal encargado;

	char archivoFinal[255];

	int salteos = 0;

	for(i = 0; i < cantidad; ++i){
		int esEncargado;

		zrecv(socket_yama, &esEncargado, sizeof(esEncargado), 0);

		if(esEncargado == 1){
			zrecv(socket_yama, encargado.nombreNodo, sizeof(char) * 100, 0);
			zrecv(socket_yama, encargado.ip, sizeof(char) * 20, 0);
			zrecv(socket_yama, &encargado.puerto, sizeof(encargado.puerto), 0);
			zrecv(socket_yama, encargado.archivoReducido, sizeof(char) * 255, 0);

			salteos++;

			log_info(logger, "NODO:%s\nIP:%s\nPUERTO:%d\nARCHIVO:%s\nENCARGADO:SI\n", encargado.nombreNodo, encargado.ip, encargado.puerto, encargado.archivoReducido);
		}else{
			zrecv(socket_yama, operacionReduccion[i - salteos].nombreNodo, sizeof(char) * 100, 0);
			zrecv(socket_yama, operacionReduccion[i - salteos].ip, sizeof(char) * 20, 0);
			zrecv(socket_yama, &operacionReduccion[i - salteos].puerto, sizeof(encargado.puerto), 0);
			zrecv(socket_yama, operacionReduccion[i - salteos].archivoReducido, sizeof(char) * 255, 0);

			log_info(logger, "NODO:%s\nIP:%s\nPUERTO:%d\nARCHIVO:%s\nENCARGADO:NO\n", operacionReduccion[i - salteos].nombreNodo, operacionReduccion[i - salteos].ip, operacionReduccion[i - salteos].puerto, operacionReduccion[i - salteos].archivoReducido);
		}
	}

	zrecv(socket_yama, archivoFinal, sizeof(char) * 255, 0);

	printf("ARCHIVO FINAL:%s", archivoFinal);
}
