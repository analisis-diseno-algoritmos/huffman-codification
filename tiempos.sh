#!/bin/bash/
gcc Codi.c tiempo.c -o tiempos
./tiempos Prueba.txt > tiempos.txt
./tiempos prueba.png >> tiempos.txt
