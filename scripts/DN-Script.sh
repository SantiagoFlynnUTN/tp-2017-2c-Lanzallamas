#!/bin/sh
echo "Instalando/Compilando bibliotecas"

git clone "https://github.com/sisoputnfrba/so-commons-library"
cd so-commons-library
sudo make install
cd ..

cd ..
cd Z-Commons/Debug

make clean
make

cd ..
cd ..

echo "Compilando DataNode"

cd DataNode/Debug

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/Z-Commons/Debug

make clean
make

echo "Generando data.bin"

fallocate -l 50M /home/utnso/data.bin

echo "Ejecutando DataNode"
cd ..

#./Debug/DataNode

exit 0
