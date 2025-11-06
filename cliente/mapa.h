#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <conio.h>

#include "cola.h"
#include "lista.h"
#include "estructuras.h"

#define SIN_MEM 0

#define PARED '#'
#define CAMINO '.'
#define ENTRADA 'E'
#define SALIDA 'S'
#define FANTASMA 'F'
#define PREMIO 'P'
#define VIDA 'V'
#define JUGADOR 'J'

#define IZQ -1
#define DER 1
#define ABJ 0

#define IZQUIERDA 'A'
#define DERECHA 'D'
#define ARRIBA 'W'
#define ABAJO 'S'

#define WIN 1
#define HP 2
#define LOSE 3
#define PUNTO 4
#define MOVIMIENTO 5

#define ABS(a) (((a) < 0) ? (-(a)) : (a))

typedef struct sJug
{
    unsigned    fil, col,
                vidas,
                puntos;
}jugador;


typedef struct mapa
{
    unsigned    filas,
                columnas,
                vidasInicio,
                maxFantasmas,
                cantFantasmas,
                maxPremios,
                maxVidasExtras,
                entrada;
    char **mapArray;
}sMapa;

typedef struct
{
    int col, fil;
}coordenada;

typedef struct
{
    coordenada posOriginal, posObjetivo;
//    char entidad;
}movimiento;

int aleatorio_entre(int min, int max);
int cargar_mapa(sMapa *mapa, char *pathParametros);

int generar_mapa(sMapa *mapa, tListaCircular *fantasmas);

void mostrar_mapa(sMapa *mapa);

int guardar_mapa(sMapa *mapa, char *pathMapaTXT);

void liberar_mapa(sMapa *mapa);

int mover_entidad(int *fil, int *col, char direccion, sMapa *mapa);

char mover(sMapa *mapa, int *fil, int *c, char direccion);

void juego(partida *nuevaPartida);

void mostrar_movimientos(sMapa *mapa, tCola *movimientos);
#endif // MAPA_H_INCLUDED
