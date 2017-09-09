/*
    ** client.c -- Ejemplo de cliente de sockets de flujo
    */

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
#include "cliente.h"
#include "main.h"
#include <commons/config.h>

#define PORT 9034 // puerto al que vamos a conectar

#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez

#define OK 1
#define ESTRUCTURA 2
#define ARCHIVO 3
#define ARCHIVOCFG "masterConfig.conf"
#define YAMA_IP "YAMA_IP"
#define YAMA_PUERTO "YAMA_PUERTO"

int sockfd;

void configs(){
	t_config * cfg;
	cfg = config_create(ARCHIVOCFG);
	if (!config_has_property(cfg, YAMA_IP) || !config_has_property(cfg, YAMA_PUERTO)){
		printf("badConfig 120");
		exit(120);
	}
	conexion.ip = config_get_string_value(cfg, YAMA_IP);
	conexion.puerto = config_get_int_value(cfg, YAMA_PUERTO);
}

void manejarDatos(int buf, int socket){
	switch(buf){
	case OK:
		printf("socket %i dice OK\n", socket);
		break;
	case ESTRUCTURA:
		//por ahora manejamos solo mensajes
		leerMensaje(socket);
		break;
	case ARCHIVO:
		printf("crear funcion para deserializar archivo\n");
		break;
	}
}

int main(int argc, char *argv[])
{
	//cargarConfig();
	//iniciarConexionAServer(sockfd, PORT);
	/* asociarAYAMA();
	 * ingresarComando();
	 * esperarIndicaciones();
	 * thread por worker: transformacion();
	 * thread por nodo: reduccion();
	 *
	 *
	 */

	configs();
	printf("La IP y puerto de YAMA son: %s:%d", conexion.ip, conexion.puerto);
/*
	for(;;);

	close(sockfd);*/
	return 0;
}
