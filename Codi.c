/*
Programa de codificación y decodificación según Huffman con el paradgima de programación voraz 
Autor: Puercos Salvajes
Para compilar y correr: Para compilar se utiliza gcc Codi.c -o nombre_de_salida_deseado.exe 
y para correrlo se escribe nombre_de_salida_deseado.exe nombre_del_archivo_a_codificar.su_extensión 
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

struct elementoA
{
    unsigned int f;
    unsigned char c;
} elementoA;

//--------------------DECLARACIÓN DE LA ESTRUCTURA  ARBOL-------------------------//
struct ArbolBB
{
    int dato;
    struct elementoA e;
    struct ArbolBB *izq;
    struct ArbolBB *der;
};

//---------DECLARACIÓN DE LA ESTRUCTURA PARA AGREGAR ELEMENTOS EN UN ÁRBOL---------------//
//Recibe una estructura de tipo ArbolAbb, un entero sin signo y un char sin signo
struct ArbolBB *AgregarElementoA(struct ArbolBB *, unsigned int f, unsigned char c);

//---------DECLARACIÓN DE LA FUNCIÓN PARA BUSCAR ELEMENTOS DENTRO DEL ÁRBOL--------//
//Recibe una estructura de tipo árbol y un entero que es el entero a buscar
void BuscarElementoA(struct ArbolBB *, int dato);

int EsHoja(struct ArbolBB *);
int Vacio(struct ArbolBB *);

int EsHoja(struct ArbolBB *raiz)
{
    return !raiz->izq && !raiz->der;
}

int Vacio(struct ArbolBB *raiz)
{
    return raiz == NULL;
}

//Especificación del método para agregar elementos a un árbol
struct ArbolBB *AgregarElementoA(struct ArbolBB *raiz, unsigned int f, unsigned char c)
{
    if (raiz == NULL) // Caso base
    {
        struct ArbolBB *nuevo = NULL;
        nuevo = (struct ArbolBB *)malloc(sizeof(struct ArbolBB));
        nuevo->e.c = c;
        nuevo->e.f = f;
        nuevo->izq = NULL;
        nuevo->der = NULL;
        return nuevo;
    }
//Si el entero que lee es menor que el que se encuentra en determinada posición, lo manda a la izquierda
    if (f < raiz->e.f)
    {
    //Llamada recursiva a la función pasándole una nueva raíz pero el mismo entero y el mismo char
        raiz->izq = AgregarElementoA(raiz->izq, f, c);
    }
//Si el entero que lee es mayor que el que se encuentra en determinada posición, lo manda a la derecha
    else
    {
        raiz->der = AgregarElementoA(raiz->der, f, c);
    }

    return raiz;
}

//Especificación de la función para buscar elementos 
void BuscarElementoA(struct ArbolBB *raiz, int dato)
{
    if (raiz == NULL)
    {
        return;
    }
    printf("\nBuscando... ");
    if (dato == raiz->dato)
        printf("\n\t%d encontrado\n", raiz->dato);
    else
    {
    //Si el entero que busca es menor que el que se encuentra en determinada posición, elegimos como nueva raíz el lado izquierdo
        if (dato < raiz->dato)
        {
        //Llamada recursiva pasándole una nueva raíz y el mismo dato a buscar
            BuscarElementoA(raiz->izq, dato);
        }
    //Si el entero que busca es mayor que el que se encuentra en determinada posición, elegimos como nueva raíz el lado derecho
        else if (dato > raiz->dato)
        {
            BuscarElementoA(raiz->der, dato);
        }
        else
        {
            printf("No se encontro el dato\n");
        }
    }
}

void RecorridoInorden(struct ArbolBB *a)
{
    if(a!=NULL)
    {
        RecorridoInorden(a->izq);
        printf("%d\n", a->e.f);
        RecorridoInorden(a->der);
    }
}

//-----------------------------------------ESTRUCTURA  LISTA--------------------------------------------------//
struct ListaSLigada
{
    int dato;
    struct ArbolBB *raiz;
    struct ListaSLigada *siguiente;
    struct ListaSLigada *anterior;
};

struct ListaSLigada *agregarElementoL(struct ListaSLigada *, struct ArbolBB *raiz);
struct ListaSLigada *mezclarListasL(struct ListaSLigada *, struct ListaSLigada *);
void mostrarLista(struct ListaSLigada *);
int buscarElementoL(struct ListaSLigada *, struct ArbolBB *raiz);

//FUNCIONES LISTA

struct ListaSLigada *agregarElementoL(struct ListaSLigada *lista, struct ArbolBB *raiz)
{
    //Agrega elementos al inicio de la lista
    struct ListaSLigada *nuevo = NULL;
    nuevo = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));
    nuevo->raiz = raiz;
    nuevo->siguiente = lista;
    return nuevo;
}

void mostrarLista(struct ListaSLigada *lista)
{
    while (lista != NULL)
    {
        printf("%d \t %d\n", lista->raiz->e.f, lista->raiz->e.c);
        lista = lista->siguiente;
    }
}

int buscarElementoL(struct ListaSLigada *lista, struct ArbolBB *raiz)
{
    struct ListaSLigada *aux = NULL;
    int band = 0;
    aux = lista;
    while (aux != NULL)
    {
        if (aux->raiz == raiz)
        {
            //printf (" %d :)! \n", aux->dato);
            band = 1;
            return 1;
            break;
        }
        aux = aux->siguiente;
    }
    if (band == 0)
    {
        //printf ("¡%d :(!! \n", dato);
        return 0;
    }
}
//-------------------------------------------ORDENAMIENTO MEZCLA------------------------------------------------//
/*FUNCION Merge
Esta funcion recibe tres parametros, un arreglo y dos enteros
Esta funcion se encargara de comparar los arreglos base e irlos
acomodando de manera que se ordenen los numeros de menor a mayor
*/
void Merge(struct elementoA A[], int p, int q, int f);
/*Funcion MergeSort
En esta funcion se reciben tres parametros, uno es el arreglo de tipo entero de 
numeros a ordenar, luego dos valores enteros que son indices. Esta funcion se 
encargara de ir partiendo el arreglo en pequenas partes hasta llegar a un solo
arreglo de un elementoL que luego sera comparado con otro arreglo de manera que 
ambos arreglos se ordenen de menor a mayor*/
void MergeSort(struct elementoA A[], int p, int f)
{
    int q;

    if (p < f)
    {
        q = trunc((p + f) / 2); //q sera el valor medio del arreglo
        MergeSort(A, p, q);     //Se ocupara la recursividad dentro de la funcion
        MergeSort(A, q + 1, f);
        Merge(A, p, q, f); //Una vez terminada la recursion se pasa a la siguiente funcion
    }
    return;
}

void Merge(struct elementoA A[], int p, int q, int f)
{
    //se declaran las variables que iran en los ciclos
    int l, i, j, k;
    l = f - p + 1;
    i = p;
    j = q + 1;

    struct elementoA *C = malloc(sizeof(elementoA) * l); //Se reserva memoria para una variable temporal
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

//-------------------------------------------GENERACION DE ARBOL DE HUFFMAN---------------------------//
struct ListaSLigada *Insercionarbol(struct ArbolBB *a, struct ListaSLigada *li);
struct ListaSLigada *generarA(struct ListaSLigada *li)
{
    struct ListaSLigada *aux = NULL;
    aux = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));
    aux = li;

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

        printf("p %d\n", arbolaux->e.f);

        printf("i %d \t %d\n", arbolaux->izq->e.c, arbolaux->izq->e.f);
        printf("d %d \t %d\n", arbolaux->der->e.c, arbolaux->der->e.f);

        aux = Insercionarbol(arbolaux, aux);
        //printf("NOESTOY\n\n\n");

        cont++;
        //printf("\n\nCONTADOR: %d\n", cont);
        aux = aux->siguiente->siguiente;
        aux2 = aux2->siguiente->siguiente;
    }
    //printf("SALI\n");
    return li;
}

//----------------------------------------------------INSERCION DEL NODO A LA LISTA-----------------------------------------------------//

struct ListaSLigada *Insercionarbol(struct ArbolBB *a, struct ListaSLigada *li)
{
    //printf("ENTREAQUI\n");
    struct ListaSLigada *aux1;

    aux1 = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));
    aux1->raiz = a;
    aux1->siguiente;
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

        while (aux2->siguiente != NULL && (aux2->raiz->e.f) < (a->e.f))
        {
            //printf("n %d %d \t %d\n", aux2->raiz->e.c, a->e.f, aux2->raiz->e.f);
            aux3 = aux2;
            aux2 = aux2->siguiente;
        }
        if (aux2->siguiente == NULL && (aux2->raiz->e.f) < (a->e.f))
        {
            //printf("P ULTIMO\n");
            aux2->siguiente = aux1;
        }
        if ((aux2->raiz->e.f) >= (a->e.f))
        {
            //printf("P INTERMEDIO\n");
            aux3->siguiente = aux1;
            aux1->siguiente = aux2;
        }
    }
    mostrarLista(li);

    return li;
}
//-----------------------------------------------ULTIMO ELEMENTO DE LA LISTA-----------------------------------//
struct ArbolBB *ultimoelemento(struct ListaSLigada *l)
{
    struct ListaSLigada *aux;
    aux = (struct ListaSLigada *)malloc(sizeof(struct ListaSLigada));

    aux=l;

    while(aux!=NULL)
    {
        aux=aux->siguiente;
        if(aux->siguiente==NULL)
        {
            return aux->raiz;
        }
    }
}
//----------------------------------------------------MAIN-----------------------------------------------------//
int main(int argc, char *argv[])
{
    struct ListaSLigada *l = NULL;
    FILE *f;
    FILE *f2; //ARCHIVO DE FRECUENCIAS EN TEXTO CLARO

    if (argc != 2)
    {
        perror("ERROR");
    }
    //--------------------------------------------LECTOR DE ARCHIVOS-----------------------------------------------//
    f = fopen(argv[1], "rb");

    fseek(f, 0L, SEEK_END);
    int tam = ftell(f);
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
    fclose(f2);
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
    /*for (i = 0; i < j; i++)
    {
        printf("%d \t %d\n", (arrSZ[i]).c, (arrSZ[i]).f);
    }*/

    //ORDENAR FRECUENCIAS
    printf("\n\n\n");
    MergeSort(arrSZ, 0, j);

    //----------------ORDENADOSSS-------------------------//
    /*for (i = 0; i < j; i++)
    {
        printf("%d \t %d\n", (arrSZ[i]).c, (arrSZ[i]).f);
    }*/

    //--------------------------------------------------
    printf("\n\n");
    struct ArbolBB *a = NULL;
    for (i = j - 1; i >= 0; i--)
    {
        a = (struct ArbolBB *)malloc(sizeof(struct ArbolBB));
        a->izq = NULL;
        a->der = NULL;
        a->e.f = arrSZ[i].f;
        a->e.c = arrSZ[i].c;
        l = agregarElementoL(l, a);
    }
    //mostrarLista(l);

    l = generarA(l);

    mostrarLista(l);
    printf("\n\n\n\n\n\n");

    
    RecorridoInorden(ultimoelemento(l));
    //-------------------------------FUNCION FRECUENCIAS--------------------------------//

    fclose(f);
    return 0;
}