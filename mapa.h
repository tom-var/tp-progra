#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
#define ABAJO 0
#define DER 1

#define IZQUIERDA 'A'
#define DERECHA 'D'
#define ARRIBA 'W'
#define ABAJO 'S'

#define WIN 1
#define HP 2
#define LOSE 3
#define POINT 4
#define MOVIMIENTO 5

typedef struct mapa
{
    unsigned    filas,
                columnas,
                vidasInicio,
                maxFantasmas,
                maxPremios,
                maxVidasExtras;
    char **mapArray;
}sMapa;

int aleatorio_entre(int min, int max);
int cargar_mapa(sMapa *mapa, FILE *archParametros);

int generar_mapa(sMapa *mapa);

void mostrar_mapa(sMapa *mapa);

int guardar_mapa(sMapa *mapa, FILE *mapaTXT);

void liberar_mapa(sMapa *mapa);

int mover_entidad(int *fil, int *col, char direccion, sMapa *mapa,int *posInicialY);


#endif // MAPA_H_INCLUDED
