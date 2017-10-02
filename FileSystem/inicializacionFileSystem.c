#include "inicializacionFileSystem.h"
#include "configuracionFileSystem.h"
#include "estructurasFileSystem.h"
#include "utilidadesFileSystem.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int _cargarBitMap(DescriptorNodo * nodo);
int _recorrerArchivosDirectorio(char * nombre);
int _procesarArchivo(char * ruta, int directorio);
void _procesarBloques(t_config * archivo, Archivo * descriptorArchivo);

void cargarTablaNodos(t_config * tablaNodos){
    char ** nombresNodos = config_get_array_value(tablaNodos, NODOS);

    while(*nombresNodos){
        DescriptorNodo * newNodo = (DescriptorNodo *)malloc(sizeof(*newNodo));
        newNodo->socket = -1; // indica que no está conectado
        strcpy(newNodo->nombreNodo, *nombresNodos);
        // FALTA IP Y PUERTO y bitmap
        char totalNodoKey[105];
        getKeyTotalBloquesNodo(totalNodoKey, *nombresNodos);

        newNodo->bloques = config_get_int_value(tablaNodos, totalNodoKey);

        char libreNodoKey[105];

        getKeyBloquesLibresNodo(libreNodoKey, *nombresNodos);

        newNodo->bloquesLibres = config_get_int_value(tablaNodos, libreNodoKey);

        if(_cargarBitMap(newNodo) == 0){
            log_info(logger, "%s\nBloques:%d\nBloques Libres:%d\n", *nombresNodos, newNodo->bloques, newNodo->bloquesLibres);
            agregarNodoEnTabla(newNodo);
        }else{
            log_info(logger, "%s fue descartado por no tener bitmap", *nombresNodos);
            free(newNodo);
        }

        nombresNodos++;
    }
}

int cargarTablaDirectorio(){
    FILE * archivoDirectorio = fopen(config_get_string_value(config, PATH_TABLA_DIRECTORIO), "rb");

    if(archivoDirectorio == NULL){
        return -1;
    }

    int i = 0;
    while(i < 100){ // recorro toda la tabla
        fread(&tabla_Directorios[i], sizeof(tabla_Directorios[i]), 1, archivoDirectorio);

        if(tabla_Directorios[i].nombre[0] == '\0') break; // salgo del for porque ya no hay nada en el archivo

        if(tabla_Directorios[i].id != i){ // hubo un directorio en el medio que se borró
            int index = tabla_Directorios[i].id;
            tabla_Directorios[index] = tabla_Directorios[i];
            tabla_Directorios[i].nombre[0] = '\0'; // limipio el nombre => libero la opción
            i = index; // continúo desde el index que modifique en la tabla de directorio
        }

        listaArchivosDirectorios[i] = list_create();
        log_debug(logger, "Directorio: %s\n Padre: %s\nID:%d\n", tabla_Directorios[i].nombre, tabla_Directorios[i].padre == -1 ? "root" : tabla_Directorios[tabla_Directorios[i].padre].nombre, tabla_Directorios[i].id);
        i++;
    }

    fclose(archivoDirectorio);

    return 0;
}

int _cargarBitMap(DescriptorNodo * nodo){
    char * archivo = (char *) malloc(sizeof(char) * 255);
    sprintf(archivo, config_get_string_value(config, PATH_BITMAPS), nodo->nombreNodo);

    FILE * f = fopen(archivo, "r");

    if(f == NULL){
        return 1;
    }

    char strBitMap[nodo->bloques + 1];

    fread(strBitMap, nodo->bloques, 1, f);

    fclose(f);

    crearBitMap(nodo);

    int i;

    for(i = 0; i < nodo->bloques; ++i){
        if(strBitMap[i] == '0'){
            bitarray_clean_bit(nodo->bitmap, i);
        }else{
            bitarray_set_bit(nodo->bitmap, i);
        }
    }

    free(archivo);
    return 0;
}

int cargarTablaArchivos(){
    char rutaDirectorioArchivos[255];
    strcpy(rutaDirectorioArchivos, config_get_string_value(config, PATH_DIR_ARCHIVOS));

    DIR * directorio = opendir(rutaDirectorioArchivos);

    if(directorio == NULL){
        log_info(logger, "No se pudo encontrar el directorio %s", rutaDirectorioArchivos);
        return -1;
    }

    struct dirent * entradaDirectorio;

    while((entradaDirectorio = readdir(directorio)) != NULL){
        // ignoro directorio actual y directorio padre
        if(!strcmp(entradaDirectorio->d_name, ".")|| !strcmp(entradaDirectorio->d_name, "..")) continue;

        if(entradaDirectorio->d_type == DT_DIR) { // es un directorio?
            log_info(logger, "Directorio: %s\n", entradaDirectorio->d_name);
            int statusCode = _recorrerArchivosDirectorio(entradaDirectorio->d_name);

            if(statusCode != 0) return -1;
        }
    }

    return 0;
}

int _recorrerArchivosDirectorio(char * nombre){
    char rutaDirectorio[255];

    sprintf(rutaDirectorio, config_get_string_value(config, PATH_DIR_ARCHIVOS_FORMAT), nombre);

    DIR * directorio = opendir(rutaDirectorio);

    if(directorio == NULL){
        log_info(logger, "No se pudo encontrar el directorio %s\n", rutaDirectorio);
        return -1;
    }

    struct dirent * entradaDirectorio;

    while((entradaDirectorio = readdir(directorio)) != NULL){
        if(entradaDirectorio->d_type == DT_REG) { // es un archivo?
            char rutaArchivo[255];

            sprintf(rutaArchivo, "%s/%s", rutaDirectorio, entradaDirectorio->d_name);
            int status = _procesarArchivo(rutaArchivo, atoi(nombre));

            if(status != 0){
                log_error(logger, "El archivo %s no pudo ser cargado, el File System está corrupto\n", entradaDirectorio->d_name);
            }
            log_info(logger, "Archivo: %s\n", entradaDirectorio->d_name);
        }
    }

    return 0;
}

int _procesarArchivo(char * ruta, int directorio){
    t_config * archivo = config_create(ruta);

    if(archivo == NULL){
        return -1;
    }

    if( !config_has_property(archivo, TAMANIO) ||
        !config_has_property(archivo, TIPO)){
        return -1;
    }

    Archivo * descriptorArchivo = (Archivo *)malloc(sizeof(*descriptorArchivo));

    descriptorArchivo->tamanio = config_get_long_value(archivo, TAMANIO);

    if(!strcmp(config_get_string_value(archivo, TIPO), ARCHIVOTEXTO)){
        descriptorArchivo->tipo = TEXTO;
    }else{
        descriptorArchivo->tipo = BINARIO;
    }

    descriptorArchivo->directorioPadre = directorio;

    _procesarBloques(archivo, descriptorArchivo);

    log_info(logger, "Tamaño: %d\nTipo: %d\nDirectorio: %d",
             descriptorArchivo->tamanio,
             descriptorArchivo->tipo,
             descriptorArchivo->directorioPadre);

    calcularRuta();

    list_add(listaArchivosDirectorios[descriptorArchivo->directorioPadre], descriptorArchivo);
    return 0;
}

void _procesarBloques(t_config * archivo, Archivo * descriptorArchivo){
    int i = 0;
    char bytesBloque[14],
         bloqueCopia0[15],
         bloqueCopia1[15];

    descriptorArchivo->bloques = list_create();

    sprintf(bytesBloque, BLOQUE_I_BYTES, i);

    while(config_has_property(archivo, bytesBloque)){
        Bloque * bloque = (Bloque *) malloc(sizeof(*bloque));

        bloque->descriptor.numeroBloque = i;
        bloque->descriptor.bytes = config_get_int_value(archivo, bytesBloque);
        printf("Bytes Bloque %d: %ld\n", i, bloque->descriptor.bytes);

        sprintf(bloqueCopia0, BLOQUE_I_COPIA_0, i);
        sprintf(bloqueCopia1, BLOQUE_I_COPIA_1, i);

        char ** copia0 = config_get_array_value(archivo, bloqueCopia0);
        char ** copia1 = config_get_array_value(archivo, bloqueCopia1);

        bloque->copia0.numeroBloque = atoi(copia0[1]);
        strcpy(bloque->copia0.nodo, copia0[0]);

        bloque->copia1.numeroBloque = atoi(copia1[1]);
        strcpy(bloque->copia1.nodo, copia1[0]);

        printf("Copia 0:\nNodo: %s\nBloque: %d\n", bloque->copia0.nodo, bloque->copia0.numeroBloque);
        printf("Copia 1:\nNodo: %s\nBloque: %d\n", bloque->copia1.nodo, bloque->copia1.numeroBloque);

        i++;
        sprintf(bytesBloque, BLOQUE_I_BYTES, i);
    }
}