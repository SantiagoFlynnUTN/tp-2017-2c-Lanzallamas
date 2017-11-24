#include "utilidadesFileSystem.h"
#include <string.h>
#include <stdlib.h>
#include <commons/string.h>

void _agregarAlPrincipio(char* s, char* t);
int _buscarDirectorio(char * nombre);

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

void registrarArchivo(Archivo * descriptorArchivo){
    if(dictionary_get(archivos, descriptorArchivo->ruta)){
        dictionary_remove_and_destroy(archivos, descriptorArchivo->ruta, free);
    }

    dictionary_put(archivos, descriptorArchivo->ruta, descriptorArchivo);

    list_add(listaArchivosDirectorios[descriptorArchivo->directorioPadre], descriptorArchivo);
}

char * obtenerNombreArchivo(char * ruta){
    char ** pathsElements = string_split(ruta, "/");
    while(*pathsElements != NULL){
        pathsElements++;
    }

    return *--pathsElements;
}

void obtenerNuevaRutaArchivo(char * rutaVieja, char * rutaDir){
    char * nombre = obtenerNombreArchivo(rutaVieja);
    strcpy(rutaVieja, rutaDir);
    strcat(rutaVieja, "/");
    strcat(rutaVieja, nombre);
}

int calcularDirectorioPadre(char * ruta){
    if(ruta[0] != '/'){ // la ruta no empieza con / por lo que no existe
        return -1;
    }

    int directorioPadre = 0;
    char ** pathsElements = string_split(ruta, "/");
    while(*(pathsElements + 1) != NULL){
        directorioPadre = _buscarDirectorio(*pathsElements);

        if(directorioPadre == -1){
            return -1;
        }

        pathsElements++;
    }

    return directorioPadre;
}

int archivoDisponible(Archivo * archivo){
	int bloques = list_size(archivo->bloques);
	int i;

	for(i = 0; i < bloques; ++i){
        Bloque * bloque = (Bloque *)list_get(archivo->bloques, i);
		DescriptorNodo * descriptorNodoCopia0 = (DescriptorNodo *) dictionary_get(nodos, bloque->copia0.nodo);
		DescriptorNodo * descriptorNodoCopia1 = (DescriptorNodo *) dictionary_get(nodos, bloque->copia1.nodo);

		if(descriptorNodoCopia0->socket == -1 && descriptorNodoCopia1->socket == -1){
			return 0;
		}
	}

	return 1;
}

int calcularEntradaDirectorio(char * dir){
    int entradaDirectorio = 0;
    Directorio directorioAnterior = tabla_Directorios[0];
    char ** pathsElements = string_split(dir, "/");
    while(*pathsElements != NULL){
        entradaDirectorio = _buscarDirectorio(*pathsElements);

        if(entradaDirectorio == -1 || directorioAnterior.id != tabla_Directorios[entradaDirectorio].padre){
            return -1;
        }

        directorioAnterior = tabla_Directorios[entradaDirectorio];
        pathsElements++;
    }

    return entradaDirectorio;
}

t_list * obtenerNombresDirectoriosHijos(int dir){
    int i;
    t_list * lista = list_create();

    for(i = 0; i < 100; ++i){
        if(tabla_Directorios[i].padre == dir && strlen(tabla_Directorios[i].nombre) > 0){
            list_add(lista, tabla_Directorios[i].nombre);
        }
    }

    return lista;
}

int cantidadDirectoriosHijos(int dir){
    int i;
    int hijos = 0;

    for(i = 0; i < 100; ++i){
        if(tabla_Directorios[i].padre == dir && strlen(tabla_Directorios[i].nombre) > 0){
            hijos++;
        }
    }

    return hijos;
}

int obtenerIdDirectorio(){
    int i;

    for(i = 0; i < 100; ++i){
        if(strlen(tabla_Directorios[i].nombre) == 0){
            return i;
        }
    }

    return -1;
}

void destruirArchivo(Archivo * descriptorArchivo){
    int bloques = list_size(descriptorArchivo->bloques);
    int i;

    for(i = 0; i < bloques; ++i){
        Bloque * descriptorBloque = (Bloque *)list_get(descriptorArchivo->bloques, i);

        liberarBloque(descriptorBloque->copia0);
        liberarBloque(descriptorBloque->copia1);

        free(descriptorBloque);
    }

    list_clean(descriptorArchivo->bloques);
    list_destroy(descriptorArchivo->bloques);

    free(descriptorArchivo);
}

void liberarBloque(Ubicacion bloque){
    if(bloque.numeroBloque < 0){
        return;
    }

    DescriptorNodo * nodo = (DescriptorNodo *) dictionary_get(nodos, bloque.nodo);
    bitarray_clean_bit(nodo->bitmap, bloque.numeroBloque);

    nodo->bloquesLibres++;
}

void _agregarAlPrincipio(char* stringOriginal, char* stringAAgregarAlPrincipio) {
    int longitud = strlen(stringAAgregarAlPrincipio);
    int i;

    memmove(stringOriginal + longitud, stringOriginal, strlen(stringOriginal) + 1);

    for (i = 0; i < longitud; ++i) {
        stringOriginal[i] = stringAAgregarAlPrincipio[i];
    }
}

int _buscarDirectorio(char * nombre){
    int i = 0;

    while(i < 100){
        if(strcmp(tabla_Directorios[i].nombre, nombre) == 0){
            return i;
        }
        i++;
    }

    return -1;
}