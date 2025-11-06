#include "lista.h"

void crear_lista(tListaCircular *lista)
{
    *lista = NULL;
}


int agregar_lista_circular(tListaCircular *lista, void *dato, unsigned tamDato)
{
    tNodo *nue = (tNodo*)malloc(sizeof(tNodo));
    if (!nue)
    {
        return -1;
    }
    nue->info = malloc(tamDato);
    nue->tam = tamDato;
    if (!nue->info)
    {
        free(nue);
        return -1;
    }
    memcpy(nue->info, dato, tamDato);

    if (*lista == NULL)
    {
        nue->sig = nue;
        *lista = nue;
        return 1;
    }
    nue->sig = (*lista)->sig;
    (*lista)->sig = nue;
    (*lista) = nue;

    return 1;
}

int sacar_lista_circular(tListaCircular *lista, void *salida, unsigned tamDato)
{
    tNodo *elim;
    if (*lista == NULL)
        return 0;
    elim = (*lista)->sig;
    memcpy(salida, elim->info, MINIMO(tamDato, elim->tam));
    if (elim == *lista)
    {
        *lista = NULL;
    }
    else
    {
        (*lista)->sig = elim->sig;
    }
    free(elim->info);
    free(elim);
    return 1;
}

int ver_lista_circular(tListaCircular *lista, void *salida, unsigned tamDato)
{
    tNodo *ver;
    if (*lista == NULL)
        return 0;
    ver = (*lista)->sig;
    memcpy(salida, ver->info, MINIMO(tamDato, ver->tam));
    return 1;
}

int avanzar_lista(tListaCircular *lista)
{
    if (*lista == NULL)
    {
        return 0;
    }
    *lista = (*lista)->sig;
    return 1;
}

int sacar_de_lista_valor(tListaCircular *lista, void *dato, int(*cmp)(void *dato1, void *dato2))
{
    tNodo *aux, *elim;
    if (*lista == NULL)
        return 0;
    aux = *lista;
    elim = aux->sig;

    while(cmp(dato, elim->info) == 0 && elim != *lista)
    {
        aux = elim;
        elim = aux->sig;
    }
    if (cmp(dato, elim->info) == 0) //No encontró elemento
        return 0;
    if (aux == elim)
        *lista = NULL;
    else
    {
        aux->sig = elim->sig;
        if (elim == *lista)
        {
            *lista = aux;
        }
    }
    free(elim->info);
    free(elim);
    return 1;
}

int sobreescribir_lista(tListaCircular *lista, void *dato, unsigned tamDato)
{
    tNodo *reemp;
    if (*lista == NULL)
    {
        return 0;
    }
    reemp = (*lista)->sig;

    free(reemp->info);
    reemp->info = malloc(tamDato);
    reemp->tam = tamDato;
    memcpy(reemp->info, dato, tamDato);
    return 1;
}

void vaciar_lista(tListaCircular *lista)
{
    tNodo *elim;
    if (!*lista)
        return;

    elim = (*lista)->sig;

    while(elim != elim->sig)
    {
        (*lista)->sig = elim->sig;
        free(elim->info);
        free(elim);
        elim = (*lista)->sig;
    }
    free(elim->info);
    free(elim);
}
