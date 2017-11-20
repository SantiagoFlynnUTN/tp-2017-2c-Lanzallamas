#!/bin/sh
echo "Copiando Repo Git"

git clone "https://github.com/sisoputnfrba/tp-2017-2c-Lanzallamas.git"
cd tp-2017-2c-Lanzallamas

echo "Instalando/Compilando bibliotecas"

git clone "https://github.com/sisoputnfrba/so-commons-library"
cd so-commons-library
sudo make install
cd ..

cd Z-Commons

gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"archivos.d" -MT"archivos.d" -o "archivos.o" archivos.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"cosas.d" -MT"cosas.d" -o "cosas.o" cosas.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"prueba.d" -MT"prueba.d" -o "prueba.o" prueba.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"sockets.d" -MT"sockets.d" -o "sockets.o" sockets.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"structUtiles.d" -MT"structUtiles.d" -o "structUtiles.o" structUtiles.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"protocoloComunicacion.d" -MT"protocoloComunicacion.d" -o "protocoloComunicacion.o" protocoloComunicacion.h
gcc -shared -o "libZ-Commons.so"  ./archivos.o ./cosas.o ./prueba.o ./sockets.o ./structUtiles.o ./protocoloComunicacion.o

echo "Compilando DataNode"

cd ..
cd DataNode

gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"chat.d" -MT"chat.d" -o "chat.o" chat.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"cliente.d" -MT"cliente.d" -o "cliente.o" cliente.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"conexionesDataNode.d" -MT"conexionesDataNode.d" -o "conexionesDataNode.o" conexionesDataNode.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"dataNode.d" -MT"dataNode.d" -o "dataNode.o" dataNode.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"mainDN.d" -MT"mainDN.d" -o "mainDN.o" mainDN.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"operacionesDataNode.d" -MT"operacionesDataNode.d" -o "operacionesDataNode.o" operacionesDataNode.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"serializacionDataNode.d" -MT"serializacionDataNode.d" -o "serializacionDataNode.o" serializacionDataNode.c
gcc -L"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug" -o "DataNode"  ./chat.o ./cliente.o ./conexionesDataNode.o ./dataNode.o ./mainDN.o ./operacionesDataNode.o ./serializacionDataNode.o   -lZ-Commons -lcommons -lpthread

echo "Ejecutando DataNode"

./DataNode

exit 0
