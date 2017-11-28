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

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug

make clean
make

echo "Generando data.bin"

fallocate -l 50M /home/utnso/data.bin

echo "Listo DataNode"
cd ..
cd ..

sudo apt-get install libreadline6 libreadline6-dev

echo "Compilando FileSystem"

cd FileSystem/Debug

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug

make clean
make

echo "Listo FileSystem"
cd ..
cd ..

echo "Compilando Master"

cd Master/Debug

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug

make clean
make

echo "Listo Master"
cd ..
cd ..

echo "Compilando Worker"

cd Worker/Debug

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug

make clean
make

echo "Listo Worker"
cd ..
cd ..

echo "Compilando YAMA"

cd YAMA/Debug

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug

make clean
make

echo "Listo YAMA"
cd
sed -i '$ a export LD_LIBRARY_PATH=$LIBRARY_PATH:/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons/Debug' .bashrc
sed -i '$ a export LC_ALL=C' .bashrc


#./Debug/DataNode

exit 0
