#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

typedef struct sNodo
{
    void *info;
    unsigned tam;
    struct sNodo *sig;
}tNodo;

typedef struct
{
    char nombreJugador[10];
    unsigned    puntaje,
                movimientos;
}partida;

#endif // ESTRUCTURAS_H_INCLUDED
