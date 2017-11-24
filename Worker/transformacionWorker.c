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
#include <sys/mman.h>
#include <fcntl.h>

void _guardarBloqueComoTemporal(char * tempFile, int numBloque, int bytes);

void _guardarBloqueComoTemporal(char * tempFile, int numBloque, int bytes){
	char bloque[bytes + 1];

	int fd = open(config_get_string_value(config, RUTA_DATABIN), O_RDONLY);

	if(fd == -1){
		log_error(logger, "No se encontró el data.bin en la ruta %s\n", infoNodo.rutaDataBin);
		return;
	}

	mapeoDataBin = mmap(NULL, MB,
						PROT_READ, MAP_PRIVATE, fd, MB * numBloque);

	if(mapeoDataBin == MAP_FAILED){
		log_error(logger, "No se pudo mapear el data.bin en la ruta\n");
		return;
	}

	strncpy(bloque, mapeoDataBin, bytes);
	memset(bloque + bytes, 0, sizeof(char));

	FILE * tempFD = fopen(tempFile, "w");

	fwrite(bloque, sizeof(char) * bytes, 1, tempFD);

	fclose(tempFD);
	munmap(mapeoDataBin, MB); // libero el mapeo
	close(fd);
}

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
	sprintf(ruta, "scripts/transformacion%d.sh", getpid());

	recibirArchivo(socket, ruta);

	char tempFile[255];
	sprintf(tempFile, "temp/transformacion%d.sh", getpid());

	_guardarBloqueComoTemporal(tempFile, t.bloque, t.cantidadBytes);

	char command[512];
	sprintf(command, "cat %s | %s | sort >> %s \n", tempFile, ruta, t.nombreTemp);

	int num = 0;

	num = system(command);

	zsend(socket, &num, sizeof(int), 0);
	exit(1);
}
