/*
 * funcionesConsolaFS.h
 *
 *  Created on: 15/9/2017
 *      Author: utnso
 */

#ifndef FUNCIONESCONSOLAFS_H_
#define FUNCIONESCONSOLAFS_H_


int yama_mkdir(char* path_dir);


//FUNCIONES AUX
char *yamaGetNFile(char *path);
char *yamaGetPathPadre(char *path);
Directorio yamaCrearDirectorio(int id, char* nombre, int id_padre);
void asignarEspacioEnTabla(Directorio newDir);
void hiloConsola();
void crear_hilo_consola();

#endif /* FUNCIONESCONSOLAFS_H_ */
