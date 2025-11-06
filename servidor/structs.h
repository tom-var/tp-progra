#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#define MIN(X,Y) ((X)<(Y)?(X):(Y))
typedef struct
{
    char nombre[6];
    unsigned    mejorPuntaje,
                partidasJugadas;
}jugador;

typedef struct
{
    char nombreJugador[10];
    unsigned    puntaje,
                movimientos;
}partida;

typedef struct sNodo
{
    struct sNodo *sig;
    unsigned tamDato;
    void *dato;
}tNodo;

#endif // STRUCTS_H_INCLUDED
