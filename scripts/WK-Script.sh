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
gcc -shared -o "libZ-Commons.so"  ./archivos.o ./cosas.o ./protocoloComunicacion.o ./prueba.o ./sockets.o ./structUtiles.o

echo "Compilando Worker"

cd ..
cd Worker

gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"RGWorker.d" -MT"RGWorker.d" -o "RGWorker.o" RGWorker.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"apareo.d" -MT"apareo.d" -o "apareo.o" apareo.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"chat.d" -MT"chat.d" -o "chat.o" chat.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"cliente.d" -MT"cliente.d" -o "cliente.o" cliente.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"enviarArchivo.d" -MT"enviarArchivo.d" -o "enviarArchivo.o" enviarArchivo.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"mainApareo.d" -MT"mainApareo.d" -o "mainApareo.o" mainApareo.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"mainWorker.d" -MT"mainWorker.d" -o "mainWorker.o" mainWorker.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"reduccionWorker.d" -MT"reduccionWorker.d" -o "reduccionWorker.o" reduccionWorker.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"serializacionWorker.d" -MT"serializacionWorker.d" -o "serializacionWorker.o" serializacionWorker.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"servidor.d" -MT"servidor.d" -o "servidor.o" servidor.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"transformacionWorker.d" -MT"transformacionWorker.d" -o "transformacionWorker.o" transformacionWorker.c
gcc -L"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug" -o "Worker"  ./RGWorker.o ./apareo.o ./chat.o ./cliente.o ./enviarArchivo.o ./mainApareo.o ./mainWorker.o ./reduccionWorker.o ./serializacionWorker.o ./servidor.o ./transformacionWorker.o   -lZ-Commons -lcommons -lpthread

echo "Ejecutando Worker"

./Worker

exit 0
