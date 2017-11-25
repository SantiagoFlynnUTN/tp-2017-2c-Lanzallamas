#!/bin/sh
echo "Instalando/Compilando bibliotecas"

sudo apt-get install libreadline6 libreadline6-dev

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
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/Z-Commons/Debug
echo "Compilando FileSystem"


cd FileSystem/Debug

make clean
make

echo "Ejecutando FileSystem"
cd ..

#./Debug/FileSystem

exit 0
