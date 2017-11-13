#include "worker.h"
#include <sockets.h>
#include <protocoloComunicacion.h>
#include "apareo.h"
#include "reduccionWorker.h"
#include "transformacionWorker.h

void iniciarReduccion(int socket) {

	int cantTemporales;
	char nombreArchivoReducido[255];

	zrecv(socket, &cantTemporales, sizeof(cantTemporales), 0);
	char nombreArchivo[cantTemporales][255];

	int i;
	for (i = 0; i < cantTemporales; ++i) {
		zrecv(socket, nombreArchivo[i], sizeof(char) * 255, 0);
	}
	zrecv(socket, nombreArchivoReducido, sizeof(char) * 255, 0);

	char ruta[255];
	sprintf(ruta, "scripts/reduccionLocal%d.sh", getpid());

	recibirArchivo(socket, ruta);

	char tempFile[255];

	for (i = 0; i < (cantTemporales - 1); ++i) {
		memset(tempFile, 0, sizeof(char)*255);
		sprintf(tempFile, "%s-%d", nombreArchivoReducido, i);
		apareo(nombreArchivo[i], nombreArchivo[i + 1], tempFile);
		strcpy(nombreArchivo[i + 1], tempFile);
	}
	log_info("nombre del archivo apareado: %s\n", tempFile);

	//rutinaTransformacion
	char command[512];
	sprintf(command, "cat %s | %s >> %s \n", tempFile, ruta, nombreArchivoReducido);

	int num = 0;

	num = system(command);

	zsend(socket, &num, sizeof(int), 0);
	exit(1);
}
