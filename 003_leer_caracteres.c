#include <stdio.h>
#include <stdlib.h>

//Muñoz González Daniel Aurelio
//Programas para leer desde la entrada

int main(){

    int c, prueba = 0;
//getchar obtiene un caracter de una cadena introducida
    //prueba = getchar();
//putchar despliega el contenido de una variable int como si fuera char
    //putchar(prueba);
/* Esto lee cada caracter de una cadena y la imprime inmediamente despues para luego repetir hasta que se obtenga el valor "EOF", que significa, end of file
el EOF es un define que se detecta como el final de un "archivo/cadena" */
    //printf("%d", (getchar() != EOF));
    //printf("%d", EOF);
    while(c != EOF){
        c = getchar();
        putchar(c); 
        if(c != '\n')
            printf("-");
        //if(c == )
     
    }
    return 0;
}