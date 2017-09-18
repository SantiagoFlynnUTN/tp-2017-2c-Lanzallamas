/*
 * transformacion.c
 *
 *  Created on: 17/9/2017
 *      Author: utnso
 */

#include "transformacionWorker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "chat.h"
#include "worker.h"
#include "conexionesWorker.h"
#include <sys/wait.h>
#include "cliente.h"

void handshake(int *sock){
	Handshake h;
	h.handshake = 1;
	h.parentPid = getppid();
	h.childPid = getpid();
	printf("%d %d %d\n", h.handshake, h.parentPid, h.childPid);
	if (send(*sock, &h, sizeof(h), 0) == -1) printf("No puedo enviar\n");
}

void hiloTransformacion(){
	int socket_master;
	conectarAHiloMaster(&socket_master);
	handshake(&socket_master);
	sleep(10);
	exit(1);
	//termina la transformacion, ya no necesito seguir el hilo
}

void forkear(int socket_master){
	int cantidadWorkers = 0;
	pid_t childPid = 1;
	recv(socket_master, &cantidadWorkers, sizeof(int), 0);

	printf("cantidadWorkers: %d\n", cantidadWorkers);
	int hijos[cantidadWorkers];

	while(cantidadWorkers && childPid){
		childPid = fork();
		hijos[cantidadWorkers] = childPid;
		cantidadWorkers--;
		sleep(1);  //averiguar si hay problemas de concurrencia.
	}
	//hijo
	if(!childPid){
		close(socket_master);
		hiloTransformacion();
	}

	//padre
	else {
		int i;
		for(i = 0; i < cantidadWorkers; i++)
			printf("Hijo %d: pid %d\n", i,  hijos[i]);
	}//sigue con el select
}


