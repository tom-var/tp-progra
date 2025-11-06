#include "arbol.h"

void crearArbolBinBusq(tArbolBinBusq *p)
{
    *p = NULL;
}

int insertarArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned tam,
                         int (*cmp)(const void *, const void *))
{
    tNodoArbol *nue;
    int         rc;
    while(*p)
    {
        if((rc = cmp(d, (*p)->info)) < 0)
            p = &(*p)->izq;
        else if (rc > 0)
            p = &(*p)->der;
        else
            return CLA_DUP;
    }

    if(!reservarMemoriaNodo(nue, sizeof(tNodoArbol), nue->info, tam))
        return SIN_MEM;
    nue->tamInfo = tam;
    memcpy(nue->info, d, tam);
    nue->der = nue->izq = NULL;
    *p = nue;
    return TODO_BIEN;
}

tNodoArbol ** buscarNodoArbolBinBusq(const tArbolBinBusq * p, const void * d,
                                     int (*cmp)(const void*, const void *))
{
    int rc;
    while(*p && (rc=cmp(d, (*p)->info)))
    {
        if(rc<0)
            p = &(*p)->izq;
        else
            p = &(*p)->der;
    }
    if(!*p)
        return NULL;
    return (tNodoArbol **)p;
}

int buscarElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned tam,
                             int (*cmp)(const void *, const void *))
{
    if(!(p = buscarNodoArbolBinBusq(p,d,cmp)))
        return 0; ///NO_EXISTE
    memcpy(d, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return 1;
}

int leerDesdeArchivoBin(void* info, void * pf, unsigned pos, size_t tam)
{
    fseek((FILE*)pf, pos*tam, SEEK_SET);
    return fread(info, tam, 1, (FILE*)pf)!=EOF;
}

int cargarArchivoBinOrdenadoArbolBinBusq(tArbolBinBusq * p, const char * path,
                                         unsigned tamInfo)
{
    int cantReg,
        r;
    FILE * pf;
    if(*p)
        return SIN_INICIALIZAR;
    if(!(pf= fopen(path, "rb")))
        return ERROR_ARCH;
    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf)/tamInfo;;
    r = cargarDesdeDatosOrdenadosRec(p, pf, leerDesdeArchivoBin, 0, cantReg-1, tamInfo);
    fclose(pf);
    return r;
}

int cargarDesdeDatosOrdenadosRec(tArbolBinBusq * p, void * ds,
                                 int (*leer)(void *, void *, unsigned, size_t tam),
                                 int li, int ls, size_t tam)
{
    int m = (li+ls)/2,
        r;
    if (li>ls)
        return TODO_BIEN;

    if (!reservarMemoriaNodo(*p, sizeof(tNodoArbol), (*p)->info, tam))
        return SIN_MEM;
    if (!leer((*p)->info, ds, m, tam))
    {
        free((*p)->info);
        free(*p);
        return ERROR_ARCH; // ERROR LECTURA
    }
    (*p)->tamInfo = tam;
    (*p)->izq = (*p)->der = NULL;

    if((r=cargarDesdeDatosOrdenadosRec(&(*p)->izq, ds, leer, li, m-1, tam))!=TODO_BIEN)
        return r;
    return cargarDesdeDatosOrdenadosRec(&(*p)->der, ds, leer, m+1, ls, tam);
}

void recorrerEnOrdenRecArbolBinBusq(const tArbolBinBusq * p, unsigned n, void * params,
                                    void (*accion)(void *, unsigned, unsigned, void *))
{
    if(!*p)
        return;
    recorrerEnOrdenRecArbolBinBusq(&(*p)->izq, n+1, params, accion);
    accion((*p)->info, (*p)->tamInfo, n, params);
    recorrerEnOrdenRecArbolBinBusq(&(*p)->der, n+1, params, accion);
}

void guardarArbolEnOrden(const tArbolBinBusq *arbol, FILE *arch)
{
    if (!*arbol)
    {
        return;
    }
    guardarArbolEnOrden(&(*arbol)->izq, arch);
    fwrite((*arbol)->info, (*arbol)->tamInfo, 1, arch);
    guardarArbolEnOrden(&(*arbol)->der, arch);
}

void recorrerOrden(const tArbolBinBusq *arbol, void(*accion)(void *))
{
    if (!*arbol)
    {
        return;
    }
    recorrerOrden(&(*arbol)->izq, accion);
    accion((*arbol)->info);
    recorrerOrden(&(*arbol)->der, accion);
}

void liberar_arbol(tArbolBinBusq *arbol)
{
    if (!*arbol)
    {
        return;
    }
    liberar_arbol(&(*arbol)->izq);
    liberar_arbol(&(*arbol)->der);

    free((*arbol)->info);
    free(*arbol);
    *arbol = NULL;

}
