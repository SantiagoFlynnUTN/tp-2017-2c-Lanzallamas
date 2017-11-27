#include "sockets.h"

/**
 * @NAME: zrecv
 * @DESC: recibe controlando que no se produzcan errores.
 *
 */
void zrecv(int socket, void* buffer, int size, int flags){
	int bytes;
	bytes = recv(socket, buffer, size, MSG_WAITALL);
	if(bytes == 0) printf("recv error: %d\n", bytes);
	if(bytes == -1) printf("recv error: %d\n", bytes);
}

/**
 * @NAME: zrecv
 * @DESC: envia controlando que no se produzcan errores.
 *
 */
void zsend(int socket, void* buffer, int size, int flags){
	if (send(socket, buffer, size, flags) == -1) {
		printf("No se puedo enviar el mensaje.\n");
	}
}

/* sendAll y recvAll: Te aseguran que se envio o recibio tod-o.
 * Respetan la misma interfaz que send y recv originales*/
int sendAll(int fd, char *cosa, int size, int flags){

	int cant_enviada = 0;
	int aux;

	zsend(fd, &size, sizeof(size), 0);

	while(cant_enviada < size){

		aux = send(fd, cosa + cant_enviada, size - cant_enviada, flags);

		if(aux == -1){
			return -1;
		}else if(aux == 0){
			return -1;
		}

		cant_enviada += aux;
	}

	return cant_enviada;
}

int recvAll(int fd, char *buffer, int size, int flags){

	int cant_recibida = 0;
	int aux;

	while(cant_recibida < size){

		aux = recv(fd, buffer + cant_recibida, size - cant_recibida, flags);

		if(aux == -1){
			return -1;
		}else if(aux == 0)
			return 0;

		cant_recibida += aux;
	}

	return cant_recibida;
}
