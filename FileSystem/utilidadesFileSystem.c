#include "utilidadesFileSystem.h"
#include <string.h>
#include <stdlib.h>
#include <commons/string.h>

void _agregarAlPrincipio(char* s, char* t);

void calcularRuta(Archivo descriptorArchivo, char * nombreArchivo, char * rutaArchivo){
    int directorio = descriptorArchivo.directorioPadre;
    memset(rutaArchivo, 0, 255);
    while(directorio != 0){
        _agregarAlPrincipio(rutaArchivo, tabla_Directorios[directorio].nombre);
        _agregarAlPrincipio(rutaArchivo, "/");
        directorio = tabla_Directorios[directorio].padre;
    }
    strcat(rutaArchivo, "/");
    strcat(rutaArchivo, nombreArchivo);
}

void registrarArchivo(Archivo * descriptorArchivo, char * ruta){
    if(dictionary_get(archivos, descriptorArchivo->ruta)){
        dictionary_remove_and_destroy(archivos, descriptorArchivo->ruta, free);
    }

    dictionary_put(archivos, descriptorArchivo->ruta, descriptorArchivo);
}

char * obtenerNombreArchivo(char * ruta){
    char ** pathsElements = string_split(ruta, "/");
    while(*pathsElements != NULL){
        pathsElements++;
    }

    return *--pathsElements;
}

void _agregarAlPrincipio(char* stringOriginal, char* stringAAgregarAlPrincipio) {
    int longitud = strlen(stringAAgregarAlPrincipio);
    int i;

    memmove(stringOriginal + longitud, stringOriginal, strlen(stringOriginal) + 1);

    for (i = 0; i < longitud; ++i) {
        stringOriginal[i] = stringAAgregarAlPrincipio[i];
    }
}
