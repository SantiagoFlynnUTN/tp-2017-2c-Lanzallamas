
#ifndef MD5_H_
#define MD5_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
* @initial_msg string del que se quiere obtener el md5
* @initial_len longitud del string initial_msg
* @MD5 un puntero a la zona de memoria dónde se va a guardar el MD5 obtenido
*/
void getMD5(char *initial_msg, size_t initial_len, char * MD5);


#endif /* MD5_H_ */