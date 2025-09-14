#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "mapa.h"
#include "cola.h"
#include "jugador.h"
#include "lista.h"

#define TOUP(X) (((X)>='a'&&(X)<='z')?(X)-32:(X))

void menu(tCola *historico,tLista *lista);
int nuevapartida(tCola *historico,tJugador *jugador);
int comparaEstructura(const void *a,const void *b);
void mostrarEstructura(const void *a);

#endif // MENU_H_INCLUDED
