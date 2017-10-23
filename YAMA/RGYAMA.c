/*
 * RGYAMA.c
 *
 *  Created on: 22/10/2017
 *      Author: utnso
 */


//implementar en serializacion.c

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sockets.h>
#include "yama.h"



void mandarNodosEjemplo(int socket_master, NodoEncargado* encargado, NodoGlobal* test){

	strcpy(encargado->nombre, "encargado");
	strcpy(encargado->ip, "127.0.0.1");
	encargado->puerto = htons(10000);
	strcpy(encargado->archTemp, "asdasd,txt");
	zsend(socket_master, encargado, sizeof(NodoEncargado), 0);

	strcpy(test[0].nombre, "encargado");
	strcpy(test[0].ip, "127.0.0.1");
	test[0].puerto = htons(10001);
	strcpy(test[0].archTemp, "asghkkkhfdrtgdfgsd,txt");

	strcpy(test[1].nombre, "encargado");
	strcpy(test[1].ip, "127.0.0.1");
	test[1].puerto = htons(10002);
	strcpy(test[1].archTemp, "asgjjhd,txt");

	int cantNodos = 2;
	zsend(socket_master, &cantNodos, sizeof(int), 0);
	zsend(socket_master, test, sizeof(NodoGlobal) * cantNodos, 0);

}

void iniciarReduccionGlobal(int socket_master){

	NodoEncargado encargado;
	NodoGlobal test[2];

	mandarNodosEjemplo(socket_master, &encargado, test);

	/*
	 *	getEncargado()
	 *	getListaNodos()
	 *	enviarEncargado()
	 *	enviarLosOtros()
	 */


}
