#include <stdio.h>
#include <stdlib.h>
#define TAMBUFFER 1000000

/***************************************/
/*         MAQUETAS DE STRUCTS         */
/***************************************/
struct Elemento {
	char* dato;
	int repeticiones;
};

struct Nodo {
	struct Elemento e;
	struct Nodo *izquierda;
	struct Nodo *derecha;
};

/***************************************/
/*       PROTOTIPOS DE FUNCIONES       */
/***************************************/
int leerArchivo(const char[]);

/**************************************/
/*            FUNCIÓN MAIN            */
/**************************************/
int main(int argc, char const *argv[]) {

	if (argc != 2) {
		printf("Escriba el nombre del archivo a comprimir - Ejemplo: %s ejemplo.png\n", argv[0]);
		return 1;
	}
	printf("Esta es una prueba para subir un commit");
	leerArchivo(argv[1]);

	return 0;
}

/***************************************/
/*       DEFINICIÓN DE FUNCIONES       */
/***************************************/
int leerArchivo(const char archivo[]) {
	FILE *fp;

	fp = fopen(archivo, "rb");
	if (!fp) {
		printf("No se pudo encontrar el archivo.");
		return 0;
	}

	char *buffer = (char*) calloc(TAMBUFFER, sizeof(char));

	fread(buffer, sizeof(struct Nodo), 1, fp);
	printf("%s\n", buffer);

	fclose(fp);

	return 1;
}
