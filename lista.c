#include "lista.h"

void crearLista(tLista *lista)
{
    *lista=NULL;
}

void vaciarLista(tLista *lista)
{

    while(*lista)
    {
        tNodo *aux=*lista;

        *lista=aux->sig;
        free(aux->info);
        free(aux);
    }
}

int listaVacia(const tLista *lista)
{
    return *lista==NULL?LISTA_VACIA:LISTA_NO_VACIA;
    //si direccion de mem de lista (1er nodo) es null entonces nonhay nodos
}

int listaLlena(const tLista *lista)
{
    return LISTA_DISPONIBLE;
}

int ponerEnListaAlFinal(tLista *lista, void*info, size_t tamInfo)
{
    tNodo *nue;
    nue=(tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return NO_MEM;
    nue->info=malloc(tamInfo);
    if(!nue->info)
    {
        free(nue);
        return NO_MEM;
    }

    memcpy(nue->info,info,tamInfo);
    nue->tam=tamInfo;
    nue->sig=NULL;

    while(*lista!=NULL)
    {
        lista=&(*lista)->sig;
    }

//    para poner en lista al principio lo unico que cambia es:
//    SIN EL while
//    nue->sig=*lista;

    *lista=nue;

    return TODO_OK;
}

int ponerEnListaAlFinalSinDuplicados(tLista *lista,void*info,size_t tamInfo,tComp accion)
{
    tNodo *nue;

    while(*lista!=NULL && accion((*lista)->info,info)!=TODO_OK)
    {
        lista=&(*lista)->sig;
    }

    if(*lista!=NULL)
        return DUPLICADO;

    nue=(tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return NO_MEM;

    nue->info=malloc(tamInfo);
    if(!nue->info)
    {
        free(nue);
        return NO_MEM;
    }

    memcpy(nue->info,info,tamInfo);
    nue->tam=tamInfo;
    nue->sig=NULL;

    *lista=nue;

    return TODO_OK;
}

int sacarUltimoDeLista(tLista*lista, void *info, size_t tamInfo)
{
    if(*lista==NULL)
        return LISTA_VACIA;

    //recorro hasta que el siguiente sean nulo (o sea es el ultimo)
    while((*lista)->sig!=NULL)
    {
        lista=&(*lista)->sig;
    }

    //Llego al ultimo con informacion y copio
    memcpy(info,(*lista)->info,MIN(tamInfo,(*lista)->tam));

    //libero porque saco el ultimo
    free((*lista)->info);
    free(*lista);
    *lista=NULL;

    return TODO_OK;
}

int verUltimoDeLista(const tLista *lista,void *info, size_t tamInfo)
{
    if(*lista==NULL)
        return LISTA_VACIA;

    while((*lista)->sig!=NULL)
    {
        lista=&(*lista)->sig;
    }

    memcpy(info,(*lista)->info,MIN(tamInfo,(*lista)->tam));

    return TODO_OK;
}

////usando siempre el primero

int ponerEnListaAlPrincipio(tLista*lista,void*info,size_t tamInfo)
{
    tNodo *nue;

    nue=(tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return NO_MEM;

    nue->info=malloc(tamInfo);
    if(!nue->info)
    {
        free(nue);
        return NO_MEM;
    }

    memcpy(nue->info,info,tamInfo);
    nue->tam=tamInfo;

    //el nodo nuevo apunta al que estaba ANTES
    //osea si inserto algo se pone al principio y el siguiente
    //apunta al que estaba antes en el principio
    nue->sig=*lista;
    *lista=nue;

    return TODO_OK;
}

int sacarPrimeroDeLista(tLista *lista,void *info,size_t tamInfo)
{
    tNodo *aux=*lista;
    if(aux==NULL)
        return LISTA_VACIA;

    //pongo la lista apuntando al sig nodo
    *lista=aux->sig;

    memcpy(info,aux->info,MIN(aux->tam,tamInfo));
    free(aux->info);
    free(aux);

    return TODO_OK;
}

int verPrimeroDeLista(const tLista *lista,void*info,size_t tamInfo)
{
    if(*lista==NULL)
        return LISTA_VACIA;
    memcpy(info,(*lista)->info,MIN((*lista)->tam,tamInfo));

    return TODO_OK;
}


void mostrarLista(tLista *lista,tAccion accion)
{
    while(*lista!=NULL)
    {
        accion((*lista)->info);
        lista=&(*lista)->sig;
    }
}



int ponerEnListaOrdenado(tLista *lista,void*info,size_t tamInfo,int op, tComp accion)
{
    tNodo *nue;
    int cmp,ban=1;

    nue=(tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return NO_MEM;

    nue->info=malloc(tamInfo);
    if(!nue->info)
    {
        free(nue);
        return NO_MEM;
    }

    ///guardo info en nue
    memcpy(nue->info,info,tamInfo);
    nue->tam=tamInfo;
    nue->sig=NULL; //es el unico que va a cambiar dado el caso


    ///RECORRE LA LISTA

    while(*lista!=NULL && ban==1)
    {
        cmp=accion(info,(*lista)->info); //compara datos

        if(cmp==TODO_OK && op==1) //son iguales y es opcion 1(sin duplicado) y 2 (con duplicado)
        {
            free(nue->info);
            free(nue);
            ban=0;
        }

        //no son iguales o pide duplicado, inserto

        if(cmp>TODO_OK) //inserto en orden descendente || <ascendente
        {
            nue->sig=*lista;
            *lista=nue;
            ban=0;
        }

        if(ban!=0)
            lista=&(*lista)->sig;
    }

    if(*lista==NULL) //si llego al ultimo, inserto al final
        *lista=nue;

    return TODO_OK;
}

