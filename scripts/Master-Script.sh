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

echo "Compilando Master"

cd ..
cd Master

gcc -I"/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"RGMaster.d" -MT"RGMaster.d" -o "RGMaster.o" RGMaster.c
gcc -I"/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"chat.d" -MT"chat.d" -o "chat.o" chat.c
gcc -I"/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"cliente.d" -MT"cliente.d" -o "cliente.o" cliente.c
gcc -I"/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"conexionesYAMA.d" -MT"conexionesYAMA.d" -o "conexionesYAMA.o" conexionesYAMA.c
gcc -I"/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"enviarArchivo.d" -MT"enviarArchivo.d" -o "enviarArchivo.o" enviarArchivo.c
gcc -I"/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"mainMaster.d" -MT"mainMaster.d" -o "mainMaster.o" mainMaster.c
gcc -I"/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"master.d" -MT"master.d" -o "master.o" master.c
gcc -I"/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"reduccionMaster.d" -MT"reduccionMaster.d" -o "reduccionMaster.o" reduccionMaster.c
gcc -I"/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"transformacionMaster.d" -MT"transformacionMaster.d" -o "transformacionMaster.o" transformacionMaster.c
gcc -L"/tp-2017-2c-Lanzallamas/Z-Commons/Debug" -o "Master"  ./RGMaster.o ./chat.o ./cliente.o ./conexionesYAMA.o ./enviarArchivo.o ./mainMaster.o ./master.o ./reduccionMaster.o ./transformacionMaster.o   -lZ-Commons -lcommons -lpthread


echo "Ejecutando Master"

./Master

exit 0
