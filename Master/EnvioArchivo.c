#include "EnvioArchivo.h"

int enviarArchivo(int kernel_fd, char* path){

	//Verifico existencia archivo (Aguante esta funcion loco!)
 	if( !verificarExistenciaDeArchivo(path) ){
 		return -1;
 	}

 	FILE* sourceFile;
 	int file_fd, file_size;
 	struct stat stats;

 	//Abro el archivo y le saco los stats
 	sourceFile = fopen(path, "r");
 	//esto nunca deberia fallar porque ya esta verificado, pero por las dudas
 	if(sourceFile == NULL){
 		log_error(logger, "No pudo abrir el archivo");
 		return -1;
 	}
 	file_fd = fileno(sourceFile);

 	fstat(file_fd, &stats);
 	file_size = stats.st_size;
 	header_t header;
 	char* buffer = malloc(file_size + 1 + sizeof(header_t));
 	memset(buffer,'@',file_size + 1 + sizeof(header_t));
 	int offset = 0;

 	if(buffer == NULL){
 		log_error(logger, "No se pudo reservar memoria para enviar archivo");
 		fclose(sourceFile);
 		return -1;
 	}

 	header.type = ENVIO_CODIGO;
 	header.length = file_size + 1;
 	memcpy(buffer, &(header.type),sizeof(header.type)); offset+=sizeof(header.type);
 	memcpy(buffer + offset, &(header.length),sizeof(header.length)); offset+=sizeof(header.length);

 	if(fread(buffer + offset,file_size,1,sourceFile) < 1){
 		log_error(logger, "No es posible leer el archivo");
 		free(buffer);
 		fclose(sourceFile);
 		return -1;
 	}

 	//Agrego \0
 	(buffer + offset)[file_size] = '\0';

 	FILE* dumpFile = fopen("Dump","w");

 	write(fileno(dumpFile),buffer,file_size + sizeof(header_t) + 1);

 	fclose(dumpFile);

 	/*Esto lo hago asi porque de la otra forma habría que reservar MAS espacio para
 	 * enviar el paquete */
 	if(sendAll(kernel_fd, buffer, file_size + sizeof(header_t) + 1, 0) <= 0){
 		log_error(logger, "Error al enviar archivo");
 		free(buffer);
 		fclose(sourceFile);
 		return -1;
 	}
 	free(buffer);
 	fclose(sourceFile);
 	return 0;
}
