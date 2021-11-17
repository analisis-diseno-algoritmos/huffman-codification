//LIBRERIAS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tiempo.h"

//MANEJO DE BITS
#define PESOBIT(bpos) 1 << bpos
#define CONSULTARBIT(var, bpos) (*(unsigned *)&var & PESOBIT(bpos)) ? 1 : 0
#define PONE_1(var, bpos) *(unsigned *)&var |= PESOBIT(bpos)
#define PONE_0(var, bpos) *(unsigned *)&var &= ~(PESOBIT(bpos))
#define CAMBIA(var, bpos) *(unsigned *)&var ^= PESOBIT(bpos)

//BOOLEAN
#define TRUE 1
#define FALSE 0

/****************************************/
/*               GLOBALES               */
/****************************************/

long long cadenas = 0;

/*****************************************/
/*              ESTRUCTURAS              */
/*****************************************/

//--------------------------------ESTRUCTURA ELEMENTO----------------------------------//
struct elementoA
{
    unsigned int f;
    unsigned char c;
} elementoA;

//----------------------------------ESTRUCTURA ARBOL----------------------------------//
struct ArbolBB
{
    int dato;
    struct elementoA e;
    struct ArbolBB *izq;
    struct ArbolBB *der;
};

//----------------------------------ESTRUCTURA LISTA----------------------------------//
struct ListaSLigada
{
    int dato;
    struct ArbolBB *raiz;
    struct ListaSLigada *siguiente;
    struct ListaSLigada *anterior;
};

/*****************************************************/
/*              PROTOTIPOS DE FUNCIONES              */
/*****************************************************/

//------------------------------------------FUNCIONES ARBOL------------------------------------------//
// struct ArbolBB *agregarElementoA(struct ArbolBB *, unsigned int, unsigned char);
// void buscarElementoA(struct ArbolBB *, int);
int esHoja(struct ArbolBB *);
int vacio(struct ArbolBB *);
void recorridoInorden(struct ArbolBB *);

//------------------------------------------FUNCIONES LISTA------------------------------------------//
// struct ListaSLigada *mezclarListasL(struct ListaSLigada *, struct ListaSLigada *);
struct ListaSLigada *agregarElementoL(struct ListaSLigada *, struct ArbolBB *);
void mostrarLista(struct ListaSLigada *);
int buscarElementoL(struct ListaSLigada *, struct ArbolBB *);

struct ListaSLigada *insercionArbol(struct ArbolBB *, struct ListaSLigada *);
struct ListaSLigada *ultimoElemento(struct ListaSLigada *);

//-----------------------------------FUNCIONES ORDENAMIENTO MEZCLA-----------------------------------//
void merge(struct elementoA [], int, int, int);
void mergeSort(struct elementoA [], int, int);

//-----------------------------------GENERACION DE ARBOL DE HUFFMAN-----------------------------------//
struct ListaSLigada *generarA(struct ListaSLigada *);

//--------------------------------------FUNCIONES DE CODIFICACIÓN-------------------------------------//
void generarCodigos(struct ArbolBB *, int , char [], char *[]);
char *obtenerCadenaArchivo(FILE *, char *[]);
long long escribirBytesM(char *);

/****************************************************************************/
/*                               FUNCIÓN MAIN                               */
/****************************************************************************/
int main(int argc, char *argv[])
{
    struct ListaSLigada *lista = NULL;
    FILE *f;
    FILE *f2; //ARCHIVO DE FRECUENCIAS EN TEXTO CLARO

    char Cindividual[256];
    char *cBytes[256];
    int nivel=0;
    long long codificador=0;

    memset(Cindividual, 0,sizeof(Cindividual));
    memset(cBytes, 0, sizeof(cBytes));

    if (argc != 2)
    {
        perror("ERROR");
    }
    //--------------------------------------------LECTOR DE ARCHIVOS-----------------------------------------------//
    double utime0, stime0, wtime0,utime1, stime1, wtime1, ttotal, tprom; //Variables para medición de tiempos
    //Inicializamos el conteo del tiempo
    uswtime(&utime0, &stime0, &wtime0);
    f = fopen(argv[1], "rb");

    fseek(f, 0L, SEEK_END);
    long long tam = ftell(f);
    fseek(f, 0L, SEEK_SET);

    unsigned char c1[256];
    unsigned int arr_aux[256];
    struct elementoA arrSZ[256];
    unsigned char aux;

    memset(c1, 0, sizeof(c1));
    memset(arr_aux, 0, sizeof(arr_aux));
    int i = 0;
    //--------------------------------------------------
    //SE OBTIENEN LOS CARACTERES Y SE GUARDAN EN UN ARREGLO AUX
    while (!feof(f))
    {
        aux = fgetc(f);
        arr_aux[aux] += 1;
        i++;
    }
    //TABLA DE FRECUENCIAS
    f2 = fopen("FRECUENCIAS.txt", "w+");
    fprintf(f2, "Byte \t FRECUENCIA\n");
    for (i = 0; i < 256; i++)
    {
        fprintf(f2, "%-1d \t %-7d \n", i, arr_aux[i]);
    }

    //SE HACE UN FILTRO DE LOS CARACTERES QUE NO APARECEN
    int j = 0;
    for (i = 0; i < 256; i++)
    {
        if (arr_aux[i] != 0)
        {
            (arrSZ[j]).c = i;
            (arrSZ[j]).f = arr_aux[i];
            j++;
        }
    }
    //----------------IMPRIMIMOS EL FILTRO-----------------/
    /*
    printf("LISTA ANTES DE MERGE\n");
    for (i = 0; i < j; i++)
    {
        printf("%d \t %d\n", (arrSZ[i]).c, (arrSZ[i]).f);
    }

    //ORDENAR FRECUENCIAS
    printf("\n\n\n");
    mergeSort(arrSZ, 0, j);
    printf("LISTA DESPUES DE MERGE\n");

    //----------------ORDENADOSSS-------------------------//
    for (i = 0; i < j; i++)
    {
        printf("%d \t %d\n", (arrSZ[i]).c, (arrSZ[i]).f);
    }
    */

    //--------------------------------------
    printf("\n\n");
    struct ArbolBB *a = NULL;
    for (i = j - 1; i >= 0; i--)
    {
        a = (struct ArbolBB *)malloc(sizeof(struct ArbolBB));
        a->izq = NULL;
        a->der = NULL;
        a->e.f = arrSZ[i].f;
        a->e.c = arrSZ[i].c;
        lista = agregarElementoL(lista, a);
    }

    lista = generarA(lista);

    // mostrarLista(lista);
    printf("\n\n\n\n\n\n");
    struct ListaSLigada *nodo;
    nodo=(struct ListaSLigada*)malloc(sizeof(struct ListaSLigada));

    nodo = ultimoElemento(lista);
    if (nodo == NULL)
    {
        printf("Hubo un error con el ultimo elemento.\n");
        return -1;
    }
    //recorridoInorden(nodo->raiz);

    generarCodigos(nodo->raiz, nivel, Cindividual, cBytes);

    for (int j = 0; j < 256; j++)
    {
        if (cBytes[j] != 0)
        {
            printf("El codigo del caracter %d  es %s\n", j,cBytes[j]);
        }
    }
    fseek(f, 0L, SEEK_SET);

    char *Aux;
    Aux = (char*) malloc(cadenas);
    Aux = obtenerCadenaArchivo(f,cBytes);

    //printf("%s ",Aux);
    codificador=escribirBytesM(Aux);
    //Finalizamos el conteo del tiempo
    uswtime(&utime1, &stime1, &wtime1);
    long long porcentaje=0;
    porcentaje= ((tam-codificador)*100)/tam;

    fprintf(f2, "%lld\n", codificador);
    fprintf(f2, "Porcentaje de compresion: %lld\n", porcentaje);

    //Calculo del tiempo de ejecucion del programa
    printf("ARCHIVO: %s\n", argv[1]);
    printf("real (Tiempo total)  %.10f s\n", wtime1 - wtime0);
    printf("user (Tiempo de procesamiento en CPU) %.10f s\n", utime1 - utime0);
    printf("sys (Tiempo en acciónes de E/S)  %.10f s\n", stime1 - stime0);
    printf("CPU/Wall   %.10f %% \n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
    printf("\n");

    //Mostrar los tiempos en formato exponecial
    printf("Formato exponencial: \n");
    printf("real (Tiempo total)  %.10e s\n", wtime1 - wtime0);
    printf("user (Tiempo de procesamiento en CPU) %.10e s\n", utime1 - utime0);
    printf("sys (Tiempo en acciónes de E/S)  %.10e s\n", stime1 - stime0);
    printf("CPU/Wall   %.10f %% \n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
    printf("-----------------------------------------------\n");
    //-------------------------------FUNCION FRECUENCIAS--------------------------------//
    fclose(f2);
    fclose(f);
    return 0;
}

/*****************************************************/
/*              DEFINICIÓN DE FUNCIONES              */
/*****************************************************/

//------------------------------FUNCIONES ARBOL------------------------------//
/*
int esHoja(struct ArbolBB *raiz)
    Entrada: Recibe una estructura arbol
    Salida: Retorna un 1 en caso de se un nodo hoja, caso contrario retorna un 0
*/
int esHoja(struct ArbolBB *raiz)
{
    if(raiz->izq==NULL && raiz->der==NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
int vacio(struct ArbolBB *raiz)
    Entrada: Recibe una estructura arbol
    Salida: retorna una raiz nula en caso de tener un arbol vacio
*/
int vacio(struct ArbolBB *raiz)
{
    return raiz == NULL;
}

/*
void recorridoInorden(struct ArbolBB *arbol)
    Entrada: Recibe una estructura arbol
    Salida: Retorna el recorrido InOrden del arbol que se encuentra en el ultimo nodo
*/
void recorridoInorden(struct ArbolBB *arbol)
{
    if(arbol!=NULL)
    {
        recorridoInorden(arbol->izq);
        printf("%d\n", arbol->e.f);
        recorridoInorden(arbol->der);
    }
}

//------------------------------FUNCIONES LISTA------------------------------//
/*
struct ListaSLigada *agregarElementoL(struct ListaSLigada *lista, struct ArbolBB *raiz)
    Entrada: Recibe una estructura lista y una estructura arbol
    Salida: Retorna una lista con un arbol raiz en su nodo
*/
struct ListaSLigada *agregarElementoL(struct ListaSLigada *lista, struct ArbolBB *raiz)
{
    //Agrega elementos al inicio de la lista
    struct ListaSLigada *nuevo = NULL;
    nuevo = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));
    nuevo->raiz = raiz;
    nuevo->siguiente = lista;
    return nuevo;
}

/*
void mostrarLista(struct ListaSLigada *lista)
    Entrada: Recibe una estructura lista
    Salida: Retorna los elementos que se encuentran en la lista
*/
void mostrarLista(struct ListaSLigada *lista)
{
    while (lista != NULL)
    {
        printf("%d \t %d\n", lista->raiz->e.f, lista->raiz->e.c);
        lista = lista->siguiente;
    }
}

/*
int buscarElementoL(struct ListaSLigada *lista, struct ArbolBB *raiz)
    Entrada: Recibe una estructura lista y una estructura arbol
    Salida: Retorna un 1 si el valor se encuentra, en caso de no ser asi retorna un 0
*/
int buscarElementoL(struct ListaSLigada *lista, struct ArbolBB *raiz)
{
    struct ListaSLigada *aux = NULL;
    // int band = 0;
    aux = lista;
    while (aux != NULL)
    {
        if (aux->raiz == raiz)
        {
            //printf (" %d :)! \n", aux->dato);
            // band = 1;
            return 1;
        }
        aux = aux->siguiente;
    }
    // if (band == 0)
    // {
        //printf ("¡%d :(!! \n", dato);
    // }
    return 0;
}

/*
struct ListaSLigada *insercionArbol(struct ArbolBB *a, struct ListaSLigada *li)
Entrada: Recibe una estructura arbol a y una estructura lista li
Salida:Retorna la lista li
Descripcion: Esta funcion se encarga de insertar los arboles (con el caracter y su frecuencia) y dependiendo de la frecuencia que
tenga el arbol, será la posicion en la que se inserte en la lista li
*/
struct ListaSLigada *insercionArbol(struct ArbolBB *arbol, struct ListaSLigada *li)
{
  //printf("ENTREAQUI\n");
  struct ListaSLigada *aux1;

  aux1 = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));
  aux1->raiz = arbol;
  if (li == NULL)
  {
    li = aux1;
  }
  else
  {
    struct ListaSLigada *aux2;
    aux2 = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));
    struct ListaSLigada *aux3;
    aux3 = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));

    aux2 = li;

    while (aux2->siguiente != NULL && (aux2->raiz->e.f) < (arbol->e.f))
    {
      //printf("n %d %d \t %d\n", aux2->raiz->e.c, a->e.f, aux2->raiz->e.f);
      aux3 = aux2;
      aux2 = aux2->siguiente;
    }
    if (aux2->siguiente == NULL && (aux2->raiz->e.f) < (arbol->e.f))
    {
      //printf("P ULTIMO\n");
      aux2->siguiente = aux1;
    }
    if ((aux2->raiz->e.f) >= (arbol->e.f))
    {
      //printf("P INTERMEDIO\n");
      aux3->siguiente = aux1;
      aux1->siguiente = aux2;
    }
  }
  mostrarLista(li);

  return li;
}

/*
struct ListaSLigada *ultimoElemento(struct ListaSLigada *l)
Entrada: Recibe una estructura lista l
Salida:Retorna una estructura lista aux
Descripcion: Esta funcion recorre la lista hasta llegar al ultimo nodo, en el cual se encuentra el arbol de huffman
con todos los pequeños arboles unidos por las frecuencias
*/
struct ListaSLigada *ultimoElemento(struct ListaSLigada *l)
{
  struct ListaSLigada *aux;
  aux = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));

  aux=l;

  while(aux!=NULL)
  {
    aux=aux->siguiente;
    if(aux->siguiente==NULL)
    {
      return aux;
    }
  }

  return NULL;
}

//------------------------------FUNCIONES ORDENAMIENTO MEZCLA------------------------------//
/*
Funcion mergeSort
En esta funcion se reciben tres parametros, uno es el arreglo de tipo entero de
numeros a ordenar, luego dos valores enteros que son indices. Esta funcion se
encargara de ir partiendo el arreglo en pequenas partes hasta llegar a un solo
arreglo de un elementoL que luego sera comparado con otro arreglo de manera que
ambos arreglos se ordenen de menor a mayor
*/
void mergeSort(struct elementoA A[], int p, int f)
{
    int q;

    if (p < f)
    {
        q = trunc((p + f) / 2); //q sera el valor medio del arreglo
        mergeSort(A, p, q);     //Se ocupara la recursividad dentro de la funcion
        mergeSort(A, q + 1, f);
        merge(A, p, q, f); //Una vez terminada la recursion se pasa a la siguiente funcion
    }
    return;
}

/*
FUNCION merge
Esta funcion recibe tres parametros, un arreglo y dos enteros
Esta funcion se encargara de comparar los arreglos base e irlos
acomodando de manera que se ordenen los numeros de menor a mayor

*/
void merge(struct elementoA A[], int p, int q, int f)
{
    //se declaran las variables que iran en los ciclos
    int l, i, j, k;
    l = f - p + 1;
    i = p;
    j = q + 1;

    struct elementoA *C = (struct elementoA*) malloc(sizeof(elementoA) * l); //Se reserva memoria para una variable temporal
    for (k = 0; k < l; k++)
    {
        if (i <= q && j <= f) //Primero se ve si los elementoLs ya han sido colocados en el arreglo
        {
            if ((A[i]).f < (A[j]).f) //Se va comparando uno a un elementoL de los arreglos
            {
                C[k] = A[i]; //Se coloca en el arreglo temporal los elementoLs de menos a mayor
                i++;
            }
            else
            {
                C[k] = A[j];
                j++;
            }
        }
        else

            if (i <= q) //En caso de que ya se encuentren todos los elementoLs colocados
        {
            C[k] = A[i]; //De igual manera de ponen en el arreglo temporal los elementoLs
            i++;
        }
        else
        {
            C[k] = A[j];
            j++;
        }
    }
    k = 0;
    //Finalmente se pasan los elementoLs del arreglo temporal al original
    for (int n = p; n <= f; n++)
    {
        A[n] = C[k];
        k++;
    }
}

//------------------------------GENERACION DE ARBOL DE HUFFMAN------------------------------//
/*
struct ListaSLigada *generarA(struct ListaSLigada *inicio)
    Entrada: Recibe una estructura Lista inicio
    Salida:Retorna la lista inicio
    Descripcion: Esta funcion se encarga de sumar los nodos de la lista, es decir, los
                pequeños arboles que se generan en cada nodo, se suman las frecuencias y se
                busca un lugar en la lista en el que se va a guardar el nuevo arbol
*/
struct ListaSLigada *generarA(struct ListaSLigada *inicio)
{
    struct ListaSLigada *aux = NULL;
    aux = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));
    aux = inicio;

    struct ListaSLigada *aux2 = NULL;
    aux2 = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));
    aux2 = aux->siguiente;
    int cont = 0;
    while (aux != NULL && aux2!= NULL)
    {
        //printf("\nVOLVI\n");
        struct ArbolBB *arbolaux = NULL;
        arbolaux = (struct ArbolBB *)malloc(sizeof(struct ArbolBB));

        arbolaux->izq = aux->raiz;
        arbolaux->der = aux2->raiz;
        arbolaux->e.f = (aux->raiz->e.f) + (aux2->raiz->e.f);

        //printf("p %d\n", arbolaux->e.f);

        //printf("i %d \t %d\n", arbolaux->izq->e.c, arbolaux->izq->e.f);
        //printf("d %d \t %d\n", arbolaux->der->e.c, arbolaux->der->e.f);

        aux = insercionArbol(arbolaux, aux);
        //printf("NOESTOY\n\n\n");

        cont++;
        //printf("\n\nCONTADOR: %d\n", cont);
        aux = aux->siguiente->siguiente;
        aux2 = aux2->siguiente->siguiente;
    }
    //printf("SALI\n");
    return inicio;
}

//------------------------------FUNCIONES DE CODIFICACIÓN------------------------------//
/*
void generarCodigos(struct ArbolBB *b, int nivel, char codigoIndividual[], char *codigosBytes[])
    Entrada: Estructura arbol b, entero nivel (indica la profundidad del arbol),  un arreglo de caracteres y un arreglo de cadenas
    Salida:
    Descripcion: La funcion recorre el arbol, segun el recorrido tendra un valor, es decir, si se va por la izquierda, su valor será 0
                en caso de que vaya derecha, es 1. Se comprueba que el nodo no sea hoja, en caso de que el nodo en el que se encuentra sea un
                caracter, guarda el recorrido para ese caracter en el arreglo.
*/
void generarCodigos(struct ArbolBB *b, int nivel, char codigoIndividual[], char *codigosBytes[])
{
    if(b->izq != NULL)
    {
        codigoIndividual[nivel] = '0';
        printf("%d\n", nivel);
        printf("%d \t i%c\n", nivel, codigoIndividual[nivel]);
        generarCodigos(b->izq,nivel+1,codigoIndividual,codigosBytes);
    }
    if(b->der !=NULL)
    {
        codigoIndividual[nivel] = '1';
        printf("%d \t d%c\n", nivel, codigoIndividual[nivel]);
        generarCodigos(b->der,nivel+1,codigoIndividual,codigosBytes);
    }
    if(esHoja(b)==1)
    {
        codigoIndividual[nivel] = '\0';
        if (codigosBytes != NULL)
        {
            codigosBytes[b->e.c] = (char*) malloc(strlen(codigoIndividual) + 1);
            printf("%s\n", codigoIndividual);
            strcpy(codigosBytes[b->e.c], codigoIndividual);
            cadenas += (strlen(codigosBytes[b->e.c])) * (b->e.f);
        }
    }
}

/*void obtenerCadenaArchivoBinario(FILE *f, char *cBytes[])
{
    int r;
    unsigned char aux;

    while (!feof(f))
    {

        aux = fgetc(f);

        if (cBytes[aux] != 0)
        {
            printf("%s", cBytes[aux]);
            //fwrite(codigosHojas[r],sizeof(char*),1,f2);
            //fprintf(f2,"%s ", codigosHojas[r]);
        }


    }

}*/
char *obtenerCadenaArchivo(FILE *f, char *cBytes[])
{
    //printf("La variable global FUNCION %d\n", cadenas);
    unsigned char aux;
    char *c;
    c = (char*) malloc(cadenas + 1);
    memset(c, 0, (size_t) sizeof(c));
    //printf("La cadena auxiliar es %s\n", c);
    while (!feof(f))
    {

        aux = fgetc(f);

        if (cBytes[aux] != 0)
        {
            strcat(c, cBytes[aux]);
            //fwrite(codigosHojas[r],sizeof(char*),1,f2);
            //fprintf(f2,"%s ", codigosHojas[r]);
        }

    }
    //printf("La cadena auxiliar RETORNADA es %s\n", c);
    return c;
    //printf("La longitud de todas las cadenas son MAIN 2 %d\n", cadenas);
}

long long escribirBytesM(char *cadena)
{
  FILE *f3;
  f3 = fopen("Frecuencia.dat", "wb+");
  printf("La variable global aqui adentro es %lld\n",cadenas);
  int longitud = strlen(cadena), contador = 0;
  long long bytes=0;
  unsigned char byte = 0;
  /*
  unsigned char byteChar = 0;

  //printf("Cadena %d\n", longitud);
  //printf("Cadena %s\n", cadena);
  */
  int bitsCompletos = (int)(longitud / 8);
  int bitsSobrantes = (longitud) - (bitsCompletos * 8);
  int siguiente = ((bitsCompletos + 1) * 8);
  int i=0;
  printf("El siguiente es %d, los completos son %d y los sobrantes son %d\n", siguiente, bitsCompletos, bitsSobrantes);

  int *castBytes;
  if (longitud % 8 == 0)
  {
    castBytes = (int*) malloc((bitsCompletos * 8) - 1);
    memset(castBytes, 0, (size_t) sizeof(castBytes));
  }

  if (longitud % 8 != 0)
  {
    castBytes = (int*) malloc((bitsCompletos * 8) + 1);
    memset(castBytes, 0, (size_t) sizeof(castBytes));
  }

  for (i = 0; i <= (bitsCompletos * 8); i++)
  {
    if (i % 8 == 0 && i > 1)
    {
      //printf("Iteracion %d\n", i);
      //printf("Byte: %c\n", byteChar);
      //fprintf(f3,"%d",castBytes[bytes]);
      fwrite(&byte, sizeof(char), 1, f3);
      bytes++;
      byte = 0;
      contador = 0;
      //printf("Se debe reiniciar la cuenta, entonces el contador es %d porque la i es %d \n",contador,i);
    }

    if (cadena[i] == '0')
    {
      //printf("Iteracion %d\n",i);
      PONE_0(byte, (7 - contador));
      //printf("0 %d\n",(int)(byte));
    }
    if (cadena[i] == '1')
    {
      //printf("Iteracion %d\n",i);
      PONE_1(byte, (7 - contador));
      //printf("1 %d\n",(int)byte);
    }
    //printf("NO se debe reiniciar la cuenta, entonces el contador es %d porque la i es %d\n",contador, i);
    contador++;
  }

  if (bitsSobrantes != 0)
  {
    byte = 0;
    contador = 0;

    //printf("Los bits sobrantes son %d y el siguiente es %d y el auxiliar %d\n",bitsSobrantes,siguiente, auxiliar);

    for (int j = i; j <= bitsSobrantes; j++)
    {
      //printf("Iteracion %d\n",contador);
      if (cadena[j] == '0')
      {
        PONE_0(byte, (7 - contador));
        //printf("0 %d\n",(int)(byte));
      }
      if (cadena[j] == '1')
      {
        PONE_1(byte, (7 - contador));
        //printf("1 %d\n",(int)byte);
      }
      //printf("NO se debe reiniciar la cuenta, entonces el contador es %d porque la i es %d\n",contador, i);
      contador++;
    }
    for (i = bitsSobrantes+1; i < siguiente; i++)
    {
      //printf("Iteracion %d\n", contador);
      PONE_0(byte, (7 - contador));
      //printf("N %d\n", (int)(byte));
      contador++;
    }
    /*
    printf("%d ", castBytes[bytes]);
    //printf("Byte: %c\n", byteChar);
    */
    fwrite(&byte, sizeof(char), 1, f3);
    /*
    printf("Byte: %d\n ", castBytes[bytes]);
    //fprintf(f3,"%d",castBytes[bytes]);
    //printf("El auxiliar de la cadena %d y el byte es %d\n", castBytes[bytes], bytes);
    */
  }
  //printf("%d\n", bytes);
  //printf("La cadena final es %s\n",cadenaBytes);
  fclose(f3);
  return bytes;
}
