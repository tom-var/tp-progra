#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED

#include "arbol.h"
#include <stdio.h>

#define TODO_OK 1
#define SIN_ESPACIO 2
#define YA_EXISTE 3

typedef struct
{
    char clave[10];
    unsigned posicion;
}regIndice;
typedef tArbolBinBusq tIndice;

//agregar a indice: Introduce elemento en arbol binario
int comparar_registros(void *valor1, void *valor2);

void crear_indice(tIndice *indice);

int insertar_indice(tIndice *indice, regIndice *registro, int (*cmp)(const void *, const void *));

int buscar_indice(tIndice *indice, regIndice *clave, int (*cmp)(const void *, const void *));

int cargar_indice(tIndice *indice, char *path);

int guardar_indice(tIndice *indice, char *path);

void liberar_indice(tIndice *indice);

#endif // INDICE_H_INCLUDED
