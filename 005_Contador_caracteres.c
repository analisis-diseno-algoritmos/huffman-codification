#include <stdio.h>
#include <stdlib.h>
//Muñoz González Daniel Aurelio 
//Programa para contabilizar determinados caracteres en un archivo de texto
//Se que es muy pronto para utilizar apuntadores, pero utilizando el cierre del programa se presentaban fallos, por lo tanto, recurrí al uso de apuntadores y funciones sencillas

int main(int argc, char const *argv[]){
    FILE *f = fopen(argv[1], "r");
    //FILE *f = fopen("Prueba.txt", "r");
    int caracteres = 0, espacios = 0, vocales = 0, lineas = 0, tabuladores = 0;
    char c;
    while (c != EOF){
        c = getc(f);
        putchar(c);
        caracteres++;
        if(c == ' ')
            espacios++;
        if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' ||  c == 'U')
            vocales++;
        if(c == '\n')
            lineas++;
        if(c == '\t')
            tabuladores++;
    }
    //printf("%s \n", c);
    printf("\nCaracteres ingresados: %d \n", caracteres);
    printf("Los espacios ingresados fueron: %d \n", espacios);
    printf("La cantidad de vocales ingresados fue: %d \n", vocales);
    printf("El total de lineas fue: %d \n", lineas);
    printf("Se introdujeron %d tabuladores \n", tabuladores);
    putchar(c);
    fclose(f);    
    return 0;
}