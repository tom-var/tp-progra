#include "jugador.h"

int mover_entidad(int *fil, int *col, char direccion, sMapa *mapa, int *posInicialY)
{

    int x = 0, y = 0;

  //  printf("DIRECCION = %i\n", direccion);
    if (direccion == IZQUIERDA)
    {
        x = 0;
        y = -1;
    }
    else if (direccion == DERECHA)
    {
        x = 0;
        y = 1;
    }
    else if (direccion == ARRIBA)
    {
        x = -1;
        y = 0;
    }
    else if (direccion == ABAJ)
    {
        x = 1;
        y = 0;
    }

 //   printf("x: %i, y: %i\n", x, y);


    if (mapa->mapArray[*fil + x][*col + y] == CAMINO)
    {
        mapa->mapArray[*fil][*col] = CAMINO;
        mapa->mapArray[*fil + x][*col + y] = JUGADOR;
        *fil = *fil + x;
        *col = *col + y;
        return MOVIMIENTO;
    }
    else if (mapa->mapArray[*fil + x][*col + y] == SALIDA)
    {
        return WIN;
    }
    else if (mapa->mapArray[*fil + x][*col + y] == FANTASMA)
    {
        mapa->mapArray[*fil][*col] = CAMINO;
        mapa->mapArray[*fil + x][*col + y] = CAMINO;
        *fil = 1;
        *col = *posInicialY; //agregue esta variable pos inicial Y para que cuando toque un fantasma vuelva al inicio
        mapa->mapArray[*fil][*col] = JUGADOR;

        return LOSE;
    }
    else if (mapa->mapArray[*fil + x][*col + y] == PREMIO)
    {
        mapa->mapArray[*fil][*col] = CAMINO;
        mapa->mapArray[*fil + x][*col + y] = JUGADOR;
        *fil = *fil + x;
        *col = *col + y;

        return POINT;
    }
    else if (mapa->mapArray[*fil + x][*col + y] == VIDA)
    {
        mapa->mapArray[*fil][*col] = CAMINO;
        mapa->mapArray[*fil + x][*col + y] = JUGADOR;
        *fil = *fil + x;
        *col = *col + y;

        return HP;
    }

    return 0;
}
