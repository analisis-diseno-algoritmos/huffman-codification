//Programa de prueba para contar caracteres
#include<stdlib.h>
#include<stdio.h>
#define CARS 500

int main(int argc, char const *argv[]){
    //FILE *f = fopen(argv[1], "r");
    FILE *f = fopen("Prueba.txt", "r");
    char arreglo1[CARS];
    int i, bandera = 2;
    /*
    if(argc != 2){
        printf("Por favor, ingrese los parámetros necesarios (nombre del archivo a comprimir)");
        return 1;
    }
    */
    char c = 'D';
    while(c != EOF){
        c = getc(f);
        for(i = 0; i < CARS; i++){ 
            /*
            if(arreglo1[0] == '/0'){         
                bandera = 1;
                break;
            }
            */
            if(c =  arreglo1[i]){
                bandera = 0;
                break;
            }
            else
                bandera = 1;     
        }
        if(bandera == 1){
            arreglo1[i] = c;
        }
    }    
    for(i = 0; i < CARS; i++){
        printf("%c \n", arreglo1[i]);
    }
}
