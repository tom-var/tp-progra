#include "indice.h"


void crear_indice(tIndice *indice)
{
    crearArbolBinBusq(indice);
}

int insertar_indice(tArbolBinBusq *indice, regIndice *registro, int (*cmp)(const void *, const void *))
{
//    printf("\n se ingresa al indice: %s %i\n", registro->clave, registro->posicion);
    return insertarArbolBinBusq(indice, (void*)registro, sizeof(regIndice), cmp);
}

int buscar_indice(tArbolBinBusq *indice, regIndice *registroBuscado, int (*cmp)(const void *, const void *))
{
    regIndice aux = *(registroBuscado);
    if(buscarElemArbolBinBusq(indice, &aux, sizeof(regIndice), cmp) == 0)
        return -1;
    else
        return aux.posicion;
}

int cargar_indice(tIndice *indice, char *path)
{
    return cargarArchivoBinOrdenadoArbolBinBusq(indice, path, sizeof(regIndice));
}

//void escribir_indice ();
void imprimir_indice(void *dato)
{
    regIndice aux = *(regIndice*)dato;
    printf("%s - %i\n", aux.clave, aux.posicion);
}

int guardar_indice(tIndice *indice, char *path)
{
    FILE *archIndice = fopen(path, "wb");
    if (archIndice == NULL)
    {
        printf("Error al escribir archivo\n");
        return -1;
    }
    guardarArbolEnOrden(indice, archIndice);
    fclose(archIndice);
    return 1;
}

void mostrar_indice(tIndice *indice)
{
    recorrerOrden(indice, imprimir_indice);
}

void liberar_indice(tIndice *indice)
{
    liberar_arbol(indice);
}
