#!/bin/sh
echo "Copiando Repo Git"

git clone "https://github.com/sisoputnfrba/tp-2017-2c-Lanzallamas.git"
cd tp-2017-2c-Lanzallamas

echo "Compilando bibliotecas"

cd Z-Commons

gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"archivos.d" -MT"archivos.d" -o "archivos.o" archivos.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"cosas.d" -MT"cosas.d" -o "cosas.o" cosas.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"prueba.d" -MT"prueba.d" -o "prueba.o" prueba.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"sockets.d" -MT"sockets.d" -o "sockets.o" sockets.c
gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"structUtiles.d" -MT"structUtiles.d" -o "structUtiles.o" structUtiles.c
gcc -shared -o "libZ-Commons.so"  ./archivos.o ./cosas.o ./prueba.o ./sockets.o ./structUtiles.o

echo "Compilando YAMA"

cd ..
cd YAMA

gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"RGYAMA.d" -MT"RGYAMA.d" -o "RGYAMA.o" RGYAMA.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"chat.d" -MT"chat.d" -o "chat.o" chat.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"clienteFS.d" -MT"clienteFS.d" -o "clienteFS.o" clienteFS.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"conexiones.d" -MT"conexiones.d" -o "conexiones.o" conexiones.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"mainYAMA.d" -MT"mainYAMA.d" -o "mainYAMA.o" mainYAMA.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"planificacionYama.d" -MT"planificacionYama.d" -o "planificacionYama.o" planificacionYama.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"respuestaTransformacion.d" -MT"respuestaTransformacion.d" -o "respuestaTransformacion.o" respuestaTransformacion.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"serializacion.d" -MT"serializacion.d" -o "serializacion.o" serializacion.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"servidor.d" -MT"servidor.d" -o "servidor.o" servidor.c
gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"yama.d" -MT"yama.d" -o "yama.o" yama.c
gcc -L"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug" -o "YAMA"  ./RGYAMA.o ./chat.o ./clienteFS.o ./conexiones.o ./mainYAMA.o ./planificacionYama.o ./respuestaTransformacion.o ./serializacion.o ./servidor.o ./yama.o   -lZ-Commons -lcommons -lpthread


echo "Ejecutando YAMA"

./YAMA

exit 0
