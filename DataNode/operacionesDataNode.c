#include "operacionesDataNode.h"
#include "dataNode.h"
#include <string.h>

int getBloque(int bloque, char data[MB]){
    /* copio el contenido del bloque en data; lo copio porque hay que liberar el mapeo y cerrar el archivo */
    strcpy(data, mapeoDataBin + bloque * MB);

    return 0;
}

int setBloque(int bloque, char data[MB]){
    /* copio la data en el mapeo del bloque */
    strcpy(mapeoDataBin + bloque * MB, data);

    // guardo el bloque
    if(msync(mapeoDataBin, infoNodo.cantidadBloques * MB, MS_SYNC) == -1){
        return -1; // error guardando el bloque
    }

    return 0;
}