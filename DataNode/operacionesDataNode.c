#include "operacionesDataNode.h"
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void getBloque(int bloque, char data[MB]){
    int fd = open("../data.bin", O_RDONLY);

    if(fd == -1){
        return -1;
    }

    char * map = mmap(NULL, MB, PROT_READ, MAP_PRIVATE, fd, bloque * MB);

    if(map == MAP_FAILED){
        return -1;
    }

    /* copio el contenido del bloque en data; lo copio porque hay que liberar el mapeo y cerrar el archivo */
    for(int i = 0; i < MB; ++i){
        data[i] = map[i];
    }

    munmap(map, MB); // libero el mapeo
    close(fd);

    return 0;
}

int setBloque(int bloque, char data[MB]){
    int fd = open("../data.bin", O_RDWR);

    if(fd == -1){
        return -1;
    }

    char * map = mmap(NULL, MB, PROT_READ | PROT_WRITE, MAP_SHARED, fd, bloque * MB);

    if(map == MAP_FAILED){
        return -1;
    }

    /* copio la data en el mapeo del bloque */
    for(int i = 0; i < MB; ++i){
        map[i] = data[i];
    }

    // guardo el bloque
    if(msync(map, MB, MS_SYNC) == -1){
        return -1; // error guardando el bloque
    }

    munmap(map, MB); // libero el mapeo
    close(fd);
    return 0;
}

/*int main(int argc, char *argv[]){
    char data[MB];
    memset(data, 'a', MB);

    setBloque(0, data);
    memset(data, 0, MB);
    getBloque(0, data);

    printf("%s", data);
}*/