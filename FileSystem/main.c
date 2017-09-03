/*
 * main.c
 *
 *  Created on: 2/9/2017
 *      Author: utnso
 */


/*
 * Conexión:
 * 		Debe levantar los DataNode hasta tener al menos 1 copia de todo.
 * 		Se eliminan todos los nodos que no esten conectados y pasa a estable.
 * 		Una vez estable permite que se conecte YAMA.
 * 		Será un servidor de multiplexado donde atenderá las peticiones de YAMA
 * 		A su vez será cliente de YAMA, pues deberá avisar cada vez q un nodo se desconecte.
 *
 * 		Tendrá tres hilos:
 * 			_Servidor
 * 			_Cliente
 * 			_Consola
 */

int main(){
	return 0;
}

