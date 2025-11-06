#include "lista.h"

void crear_lista(tLista *lista)
{
    *lista = NULL;
}
int lista_vacia(tLista *lista)
{
    return ((*lista) == NULL)?VACIA:NO_VACIA;
}
int lista_insertar_ordenado(tLista *lista, void *dato, unsigned tamDato, int (*cmp)(const void *, const void *))
{
    tNodo *nue = (tNodo*)malloc(sizeof(tNodo));
    if (nue == NULL || (nue->dato = malloc(tamDato)) == NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->dato, dato, tamDato);
    nue->tamDato = tamDato;
    if (*lista == NULL || cmp(dato, (*lista)->dato) >= 0)
    {
        nue->sig = (*lista);
        *lista = nue;
        return 1;
    }
    while((*lista)->sig != NULL && (cmp(dato, (*lista)->sig->dato) < 0))
    {
        *lista = (*lista)->sig;
    }
    nue->sig = (*lista)->sig;
    (*lista)->sig = nue;
    return 1;
}

int poner_en_orden(tLista *lista, void *dato, size_t tamDato, int (*comparar)(const void *valor1, const void *valor2))
{
    tNodo *nue = (tNodo*)malloc(sizeof(tNodo));
    nue->dato = malloc(tamDato);
    tLista aux = *lista;
    memcpy(nue->dato, dato, tamDato);
    if (!(*lista) || comparar(dato, (*lista)->dato) >= 0) // no hay elementos o es mayor que todos
    {
        *lista = nue;
        nue->sig = aux;
        return 1;
    }
   while (aux->sig != NULL && comparar(dato, aux->sig->dato) < 0)
    {
        aux = aux->sig;
    }
    nue->sig = aux->sig;
    aux->sig = nue;
    return 1;
}


int poner_al_principio(tLista *lista, void *dato, size_t tamDato, int (*comparar)(const void *valor1, const void *valor2))
{
    tNodo *nue = (tNodo*)malloc(sizeof(tNodo));
    nue->dato = malloc(tamDato);
    memcpy(nue->dato, dato, tamDato);
    nue->sig = *lista;
    *lista = nue;
    return 1;
}
int lista_eliminar_ultimo(tLista *lista)
{
    tNodo *elim;
    tLista aux = *lista;
    if (*lista == NULL)
        return 0; // pila vacia
    if ((*lista)->sig == NULL)
    {
        elim = *lista;
        free(elim->dato);
        free(elim);
        (*lista) = NULL;
    }
    while((aux)->sig->sig)
    {
        (aux) = (aux)->sig;
    }
    elim = (aux)->sig;
    (aux)->sig = NULL;
    free(elim->dato);
    free(elim);
    return 1;
}


void imprimir_lista(tLista *lista)
{
    tLista recorrer = *lista;
    printf("JUGADOR | PUNTAJE\n");
    int i = 1;
//    if (recorrer->sig == NULL)
//    {
//
//        printf("%s    | %i\n", ((jugador*)recorrer->dato)->nombre, ((jugador*)recorrer->dato)->mejorPuntaje);
//    }
    while(recorrer)
    {
        printf("%i)%s    | %i\n", i, ((jugador*)recorrer->dato)->nombre, ((jugador*)recorrer->dato)->mejorPuntaje);
        recorrer = recorrer->sig;
        i++;
    }
//    printf("%i)%s    | %i\n",i, ((jugador*)recorrer->dato)->nombre, ((jugador*)recorrer->dato)->mejorPuntaje);
}


int sacar_de_lista(tLista *lista, void *salida, unsigned tamDato)
{
    tNodo *elim;
    if ((*lista)==NULL)
        return 0;
    elim = *lista;
    *lista = elim->sig;
    memcpy(salida, elim->dato, MIN(tamDato, elim->tamDato));
    free(elim->dato);
    free(elim);
    return 1;
}
