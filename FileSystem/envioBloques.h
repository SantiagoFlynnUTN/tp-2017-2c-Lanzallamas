#ifndef TP_2017_2C_LANZALLAMAS_ENVIOBLOQUES_H
#define TP_2017_2C_LANZALLAMAS_ENVIOBLOQUES_H

#define MB 1048576

int enviarBloques(char * archivo, char * archivoYamaFS);
int generarCopia(Bloque * bloque, DescriptorNodo * descriptorNodo);

#endif //TP_2017_2C_LANZALLAMAS_ENVIOBLOQUES_H
