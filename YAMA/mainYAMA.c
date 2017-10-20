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
#include "clienteFS.h"


void nuevoCliente(char* remoteHost, int newfd){
	printf("new conection from %s on socket %d\n", remoteHost, newfd);
	//Y acá hacer algo con el nuevo cliente conectado
}


void conectarAFileSystem() {
	struct sockaddr_in their_addr; // información de la dirección de destino

	if ((sock_fs = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
	their_addr.sin_port = conexionFileSystem.puerto; // short, Ordenación de bytes de la red
	their_addr.sin_addr.s_addr = inet_addr(conexionFileSystem.ip);
	memset(&(their_addr.sin_zero), 0, 8); // poner a cero el resto de la estructura

	if (connect(sock_fs, (struct sockaddr *) &their_addr,
			sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}

int main(){
	inicializarYAMA();
	inicializarServer();
	iniciarConexionAFS(&sock_fs);

	for(;;);
	return 0;
}
