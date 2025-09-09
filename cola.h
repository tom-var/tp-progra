#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


#define MIN(X,Y) ((X)<(Y)?(X):(Y))

#define COLA_VACIA 0
#define COLA_NO_VACIA -1
#define COLA_DISPONIBLE -2
#define TODO_OK 1
#define NO_MEM -3

typedef struct sNodo
{
    void *info;
    size_t tam;
    struct sNodo *sig;
}tNodo;

typedef struct
{
    struct sNodo *ult;
    struct sNodo *pri;
}tCola;


void crearCola(tCola*cola);
int ponerEnCola(tCola *cola, void *dato, size_t tamDato);
int sacarDeCola(tCola*cola,void*dato,size_t tamDato);
int colaVacia(tCola *cola);
int colaLLena(tCola *cola);
void vaciarCola(tCola*cola);
int verPrimeroCola(tCola *cola,void *info, size_t tamInfo);

#endif // COLA_H_INCLUDED
