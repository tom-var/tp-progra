#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

#define VACIA 1
#define NO_VACIA 0


typedef tNodo *tLista;
void crear_lista(tLista *lista);

int lista_vacia(tLista *lista);

int lista_insertar_ordenado(tLista *lista, void *dato, unsigned tamDato, int (*cmp)(const void *, const void *));


int poner_en_orden(tLista *lista, void *dato, size_t tamDato, int (*comparar)(const void *valor1, const void *valor2));


int poner_al_principio(tLista *lista, void *dato, size_t tamDato, int (*comparar)(const void *valor1, const void *valor2));

int lista_eliminar_ultimo(tLista *lista);

void imprimir_lista(tLista *lista);


int sacar_de_lista(tLista *lista, void *salida, unsigned tamDato);


#endif // LISTA_H_INCLUDED

