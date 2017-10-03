#include "fileSystem.h"
#ifndef TP_2017_2C_LANZALLAMAS_INICIALIZACIONFILESYSTEM_H
#define TP_2017_2C_LANZALLAMAS_INICIALIZACIONFILESYSTEM_H

void cargarTablaNodos(t_config * tablaNodos);
int cargarTablaDirectorio();
int cargarTablaArchivos();

#define TIPO "TIPO"
#define ARCHIVOTEXTO "TEXTO"
#define ARCHIVOBINARIO "BINARIO"

#define BLOQUE_I_COPIA_0 "BLOQUE%dCOPIA0"
#define BLOQUE_I_COPIA_1 "BLOQUE%dCOPIA1"
#define BLOQUE_I_BYTES "BLOQUE%dBYTES"

#endif //TP_2017_2C_LANZALLAMAS_INICIALIZACIONFILESYSTEM_H
