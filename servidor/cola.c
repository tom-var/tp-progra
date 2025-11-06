#include "cola.h"

void crearCola(tCola*cola)
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
    printf("0\n");
    nue->dato=malloc(tamDato);
    if(!nue->dato)
    {
        free(nue);
        return NO_MEM;
    }
    printf("1\n");
    memcpy(nue->dato,dato,tamDato);
    nue->tamDato=tamDato;
    nue->sig=NULL; //esto se pone SIEMPRE
    printf("2\n");
    if(cola->ult)
        cola->ult->sig = nue;
    else
        cola->pri = nue;
    printf("3\n");
    cola->ult=nue;

    return TODO_OK;
}

int sacarDeCola(tCola*cola,void*dato,size_t tamDato)
{
    tNodo *elim;
    if(!cola->pri)
        return COLA_VACIA;

    elim=cola->pri;
    memcpy(dato,elim->dato,MIN(tamDato,elim->tamDato));

    cola->pri=elim->sig;

    free(elim->dato);
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
        free(aux->dato);
        free(aux);
    }
    cola->ult=cola->pri=NULL;
}

int verPrimeroCola(tCola *cola,void *dato, size_t tamInfo)
{
    if(cola->pri==NULL)
        return COLA_VACIA;
    memcpy(dato,cola->pri->dato,MIN(tamInfo,cola->pri->tamDato));

    return TODO_OK;
}
