#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include<string.h>
#include "estructuras.h"


#define MINIMO(x, y) ( (x) <= (y) ? (x) : (y) )


typedef tNodo *tLista;
typedef tNodo *tListaCircular;

int compararInts(const void *a, const void *b);

void crear_lista(tListaCircular *lista);
int agregar_lista_circular(tListaCircular *lista, void *dato, unsigned tamDato);
int sacar_lista_circular(tListaCircular *lista, void *salida, unsigned tamDato);
int ver_lista_circular(tListaCircular *lista, void *salida, unsigned tamDato);
int avanzar_lista(tListaCircular *lista);
int sacar_de_lista_valor(tListaCircular *lista, void *dato, int(*cmp)(void *dato1, void *dato2));
int sobreescribir_lista(tListaCircular *lista, void *dato, unsigned tamDato);

#endif // LISTA_H_INCLUDED
