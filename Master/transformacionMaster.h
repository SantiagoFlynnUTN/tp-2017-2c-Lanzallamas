#ifndef TRANSFORMACIONMASTER_H_
#define TRANSFORMACIONMASTER_H_

typedef struct{
	int tipoMensaje;
	long cantidadBytes;
	int bloque;
	char nombreTemp[255];
}__attribute__((packed))
mensajeTransformacion;

int respuestaSolicitud(int socket_yama);
void mandarTransformacionNodo(int socket_nodo, int socket_yama, int cantidadWorkers);
void atenderConexion(int socket);

#endif /* TRANSFORMACIONMASTER_H_ */
