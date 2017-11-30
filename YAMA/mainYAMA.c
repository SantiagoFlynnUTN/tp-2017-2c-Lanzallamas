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
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "planificacionYama.h"

void signal_handler(int signum){
	if (signum == SIGUSR1) {
		recargarConfiguracion();
		log_info(logger, "Recarga de configurcion OK\n");
		cabecera = 0;
	}
}

void logEstadoNodos(){
	t_list * nodos = list_create();
	list_clean(nodos);

	void getNodos(void* entrada){
		EntradaTablaEstado * en = (EntradaTablaEstado *) entrada;

		bool finder(void * nodo) {
			EntradaTablaEstado* node = (EntradaTablaEstado*) nodo;
			return !strcmp(en->nombreNodo, node->nombreNodo);
		}

		if(!list_any_satisfy(nodos, finder)) list_add(nodos, en);
	}
	list_iterate(tablaEstado, getNodos);

	log_info(logger, "Nodo\tCarga\tHistoricas");

	void logNodos(void * nodo){
		EntradaTablaEstado* en = (EntradaTablaEstado*) nodo;
		log_info(logger, "%s\t%d\t%d", en->nombreNodo, trabajoActual(en->nombreNodo), getTareasHistoricas(en->nombreNodo));
	}

	list_iterate(nodos, logNodos);
}

void signal2_handler(int sig){
	if(sig == SIGUSR2){
		logEstadoNodos();
		cabecera = 0;
	}
}

void nuevoCliente(char* remoteHost, int newfd){
	log_info(logger, "Nueva conexion de %s en socket %d", remoteHost, newfd);
	cabecera = 0;
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

void signals() {
	struct sigaction new_action, old_action;

	/* Set up the structure to specify the new action. */
	new_action.sa_handler = recargarConfiguracion;
	new_action.sa_flags = SA_RESTART;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = SA_RESTART;

	sigaction(SIGUSR1, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN)
		sigaction(SIGUSR1, &new_action, NULL);

	signal(SIGUSR2, signal2_handler);

	/*

	signal(SIGUSR1, signal_handler);
	*/
}

int main(){

	pthread_mutex_t sem = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&sem);
	signals();
	inicializarYAMA();
	iniciarConexionAFS(&sock_fs);
	log_info(logger, "Para recargar la configuracion utilice el comando kill -USR1 %d", getpid());
	inicializarServer();

	pthread_mutex_lock(&sem);
	return 0;
}
