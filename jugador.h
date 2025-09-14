#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include "mapa.h"
#include "cola.h"

typedef struct{
    int movX,movY,cantVidas,puntos;
    char alias[10];
}tJugador;

int mover_entidad(int *fil, int *col, char direccion, sMapa *mapa, int *posInicialY);

#endif // JUGADOR_H_INCLUDED
