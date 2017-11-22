#!/bin/sh
echo "Copiando Repo Git"

echo "Instalando/Compilando bibliotecas"

git clone "https://github.com/sisoputnfrba/so-commons-library"
cd so-commons-library
sudo make install
cd ..

cd ..
cd Z-Commons

gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"archivos.d" -MT"archivos.d" -o "archivos.o" archivos.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"cosas.d" -MT"cosas.d" -o "cosas.o" cosas.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"protocoloComunicacion.d" -MT"protocoloComunicacion.d" -o "protocoloComunicacion.o" protocoloComunicacion.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"prueba.d" -MT"prueba.d" -o "prueba.o" prueba.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"sockets.d" -MT"sockets.d" -o "sockets.o" sockets.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"structUtiles.d" -MT"structUtiles.d" -o "structUtiles.o" structUtiles.c
gcc -shared -o "libZ-Commons.so"  ./archivos.o ./cosas.o ./prueba.o ./sockets.o ./structUtiles.o

echo "Compilando FileSystem"

cd ..
cd FileSystem

gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"chat.d" -MT"chat.d" -o "chat.o" chat.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"conexionesFileSystem.d" -MT"conexionesFileSystem.d" -o "conexionesFileSystem.o" conexionesFileSystem.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"configuracionFileSystem.d" -MT"configuracionFileSystem.d" -o "configuracionFileSystem.o" configuracionFileSystem.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"envioBloques.d" -MT"envioBloques.d" -o "envioBloques.o" envioBloques.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"estructurasFileSystem.d" -MT"estructurasFileSystem.d" -o "estructurasFileSystem.o" estructurasFileSystem.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"fileSystem.d" -MT"fileSystem.d" -o "fileSystem.o" fileSystem.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"funcionesConsolaFS.d" -MT"funcionesConsolaFS.d" -o "funcionesConsolaFS.o" funcionesConsolaFS.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"inicializacionFileSystem.d" -MT"inicializacionFileSystem.d" -o "inicializacionFileSystem.o" inicializacionFileSystem.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"mainFS.d" -MT"mainFS.d" -o "mainFS.o" mainFS.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"recepcionBloques.d" -MT"recepcionBloques.d" -o "recepcionBloques.o" recepcionBloques.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"serializacionFileSystem.d" -MT"serializacionFileSystem.d" -o "serializacionFileSystem.o" serializacionFileSystem.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"servidor.d" -MT"servidor.d" -o "servidor.o" servidor.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"utilidadesFileSystem.d" -MT"utilidadesFileSystem.d" -o "utilidadesFileSystem.o" utilidadesFileSystem.c
gcc -L"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug" -o "FileSystem"  ./chat.o ./conexionesFileSystem.o ./configuracionFileSystem.o ./envioBloques.o ./estructurasFileSystem.o ./fileSystem.o ./funcionesConsolaFS.o ./inicializacionFileSystem.o ./mainFS.o ./recepcionBloques.o ./serializacionFileSystem.o ./servidor.o ./utilidadesFileSystem.o   -lZ-Commons -lreadline -lcommons -lpthread

echo "Ejecutando FileSystem"

./FileSystem

exit 0
