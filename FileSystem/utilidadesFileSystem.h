#include "fileSystem.h"

#ifndef TP_2017_2C_LANZALLAMAS_UTILIDADESFILESYSTEM_H
#define TP_2017_2C_LANZALLAMAS_UTILIDADESFILESYSTEM_H

void calcularRuta(Archivo descriptorArchivo, char * nombreArchivo, char * rutaArchivo);
void registrarArchivo(Archivo * descriptorArchivo);
char * obtenerNombreArchivo(char * ruta);
int calcularDirectorioPadre(char * ruta);
#endif //TP_2017_2C_LANZALLAMAS_UTILIDADESFILESYSTEM_H
