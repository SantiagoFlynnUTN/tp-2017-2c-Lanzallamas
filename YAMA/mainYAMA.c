/*
 * main.c
 *
 *  Created on: 2/9/2017
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
#include "servidor.h"
#include "chat.h"
#include "yama.h"

int sock_fs;


void nuevoCliente(char* remoteHost, int newfd){
	printf("new conection from %s on socket %d\n", remoteHost, newfd);
	//Y acá hacer algo con el nuevo cliente conectado
}


void conectarAFileSystem(){
	//rutina de conexion al FS
}

int main(){
	inicializarYAMA();
	inicializarServer();
	//conectarAFileSystem();
	//iniciarConexionAFS(&sock_fs);

	for(;;);
	return 0;
}
