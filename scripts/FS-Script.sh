#!/bin/sh
echo "Copiando Repo Git"

git clone "https://github.com/sisoputnfrba/tp-2017-2c-Lanzallamas.git"

gcc demo.c -o demo
make demo


exit 0
