#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cola.h"

#define TODO_OK 1
#define LISTA_VACIA -2
#define LISTA_NO_VACIA 2
#define DUPLICADO -3
#define LISTA_DISPONIBLE -4

#define MIN(X,Y) ((X)<(Y)?(X):(Y))


typedef tNodo * tLista;
typedef void (*tAccion)(const void*);
typedef int (*tComp)(const void*,const void*);

void crearLista(tLista *lista);
void vaciarLista(tLista *lista);
int listaVacia(const tLista *lista);
int listaLlena(const tLista *lista);

int ponerEnListaAlFinal(tLista *lista, void*info, size_t tamInfo);
int ponerEnListaAlFinalSinDuplicados(tLista *lista,void*info,size_t tamInfo,tComp accion);
int sacarUltimoDeLista(tLista*lista, void *info, size_t tamInfo);
int verUltimoDeLista(const tLista *lista,void *info, size_t tamInfo);

int ponerEnListaAlPrincipio(tLista*lista,void*info,size_t tamInfo);
int sacarPrimeroDeLista(tLista *lista,void *info,size_t tamInfo);
int verPrimeroDeLista(const tLista *lista,void*info,size_t tamInfo);

void mostrarLista(tLista *lista, tAccion accion);

int ponerEnListaOrdenado(tLista *lista,void*info,size_t tamInfo,int op, tComp accion); //op1:sin duplicado, op2:con duplicado

#endif // LISTA_H_INCLUDED
