#include "transformacionWorker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <archivos.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "chat.h"
#include "worker.h"
#include <sys/wait.h>
#include "cliente.h"
#include <sockets.h>

void recibirArchivo(int socket, char * ruta){
	int longitud;
	zrecv(socket, &longitud, sizeof(longitud), 0);

	char buffer[longitud];
	memset(buffer, 0, longitud);

	zrecv(socket, &buffer, longitud * sizeof(char), 0);

	guardarArchivo(ruta, buffer, longitud);
}

void iniciarTransformacion(int socket){
	mensajeTransf t;
	zrecv(socket, &t.cantidadBytes, sizeof(t.cantidadBytes), 0);
	zrecv(socket, &t.bloque, sizeof(t.bloque), 0);
	zrecv(socket, t.nombreTemp, sizeof(char) * 255, 0);
	char ruta[255];
	//sprintf(ruta, "scripts/transformacion%d.sh", getpid());
	recibirArchivo(socket, ruta);
	char command[255];
	sprintf(command, "echo \"hola\ncomo\nestas\nbien\nvos\ntodo\nbien\nque\nsuerte\nte\nfelicito\" | sort >> %s\n", t.nombreTemp);
	printf("EJECUTANDO %s\n", command);
	system(command);
	int num = 0;
	zsend(socket, &num, sizeof(int), 0);
	exit(1);
}

