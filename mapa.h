#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PARED '#'
#define CAMINO '.'
#define ENTRADA 'E'
#define SALIDA 'S'
#define FANTASMA 'F'
#define PREMIO 'P'
#define VIDA 'V'

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


#endif // MAPA_H_INCLUDED
