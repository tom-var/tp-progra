#include "cola.h"

void crearCola(tCola *cola)
{
    cola->pri=NULL;
    cola->ult=NULL;
}

int ponerEnCola(tCola *cola, void *dato, size_t tamDato)
{
    tNodo *nue;
    nue=malloc(sizeof(tNodo));
    if(!nue)
        return NO_MEM;
    nue->info=malloc(tamDato);
    if(!nue->info)
    {
        free(nue);
        return NO_MEM;
    }

    memcpy(nue->info,dato,tamDato);
    nue->tam=tamDato;
    nue->sig=NULL; //esto se pone SIEMPRE

    if(cola->pri==NULL)
        cola->pri=nue; //entonces es el primero
    else
        cola->ult->sig=nue; //ntonces ya hay alguien en la cola

    cola->ult=nue;

    return TODO_OK;
}

int sacarDeCola(tCola*cola,void*dato,size_t tamDato)
{
    tNodo *elim;
    if(!cola->pri)
        return COLA_VACIA;

    elim=cola->pri;
    memcpy(dato,elim->info,MIN(tamDato,elim->tam));

    cola->pri=elim->sig;

    free(elim->info);
    free(elim);
    if(cola->pri == NULL)
        cola->ult = NULL;

    return TODO_OK;
}

int colaVacia(tCola *cola)
{
    return cola->pri==NULL?COLA_VACIA:COLA_NO_VACIA;
}

int colaLLena(tCola *cola)
{
    return COLA_DISPONIBLE;
}

void vaciarCola(tCola*cola)
{
    tNodo *aux;

    while(cola->pri)
    {
        aux=cola->pri;
        cola->pri=aux->sig;
        free(aux->info);
        free(aux);
    }
    cola->ult=cola->pri=NULL;
}

int verPrimeroCola(tCola *cola,void *info, size_t tamInfo)
{
    if(cola->pri==NULL)
        return COLA_VACIA;
    memcpy(info,cola->pri->info,MIN(tamInfo,cola->pri->tam));

    return TODO_OK;
}
