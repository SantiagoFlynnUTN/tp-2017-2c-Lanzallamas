/*
 * transformacion.c
 *
 *  Created on: 17/9/2017
 *      Author: utnso
 */

#include "transformacionWorker.h"

void conectarAHiloMaster(int socket_master){

}

void forkear(int socket_master){
	int cantidadWorkers = 0;
	pid_t childPid = 1;
	recv(socket_master, &cantidadWorkers, sizeof(int), 0);

	int hijos[cantidadWorkers];

	while(cantidadWorkers && childPid){
		childPid = fork();
		hijos[cantidadWorkers] = childPid;
		cantidadWorkers--;
	}
	//hijo
	if(!childPid){
		conectarAHiloMaster(socket_master);
	}

	//padre
	else {
		for(int i = 0; i < cantidadWorkers; i++)
			printf("Hijo %d: pid %d\n", i,  hijos[i]);
	}
}


