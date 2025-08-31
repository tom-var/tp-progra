#include "mapa.h"
//typedef struct mapa
//{
//    unsigned    filas,
//                columnas,
//                vidasInicio,
//                maxFantasmas,
//                maxPremios,
//                maxVidasExtras;
//    char *mapArray;
//}sMapa;


//Generación del mapa:
// Primer corrida:
//Elegir un punto aleatorio en la primer fila y marcarlo como entrada
//Bajar un nivel de ese punto
//De ahí, elegir de manera aleatoria si ir a la izquierda, derecha o abajo. En caso de que choque contra una pared, se debe ir abajo obligatoriamente.
//Repetir este proceso hasta llegar al "piso".
//Una vez se choca con el piso, marcarlo como salida.

//Segunda pasada
//
int aleatorio_entre(int min, int max)
{
//    srand(time(NULL));
    return min + rand() % (max - min + 1);
}

int generar_mapa(sMapa *mapa)
{
    int i, j, n;
    srand(time(NULL));
    mapa->filas = 10;
    mapa->columnas = 10;
    int entrada = aleatorio_entre(1, mapa->columnas - 2);
//    int n = min + rand() % (max - min + 1);


    mapa->filas = 10;
    mapa->columnas = 10;

//    mapa->mapArray = (char**)malloc(mapa->filas * sizeof(char*));
    mapa->mapArray = (char**)calloc(mapa->filas, sizeof(char*));
    for (i = 0; i < mapa->columnas; i++)
    {
//        mapa->mapArray[i] = (char*)malloc(mapa->columnas * sizeof(char));
        mapa->mapArray[i] = (char*)calloc(mapa->columnas, sizeof(char));
    }

    //Generar paredes
    for(i = 0; i < mapa->filas; i++)
    {
        mapa->mapArray[i][0] = PARED; // i = col j = fil
        mapa->mapArray[i][mapa->filas - 1] = PARED;
        mapa->mapArray[0][i] = PARED;
        mapa->mapArray[mapa->columnas - 1][i] = PARED;
    }
    //Elegir entrada
    mapa->mapArray[entrada][0] = ENTRADA;
    mapa->mapArray[entrada][1] = CAMINO;
    j = entrada;
    i = 1;
    while(i < mapa->filas - 1)
    {
//        n = aleatorio_entre(-1, 1);
//        if (n == -1)
//        {
////            printf("%i", n);
//            //ir izq
////            mapa->mapArray[j][i] = CAMINO;
//            if (mapa->mapArray[j + n][i] == PARED)
//                n = 0;
//            else
//                mapa->mapArray[j + n][i] = CAMINO;
//        }
//        else if(n == 1)
//        {
////            printf("%i", n);
//            //ir der
//            if (mapa->mapArray[j + n][i] == PARED)
//                n = 0;
//            else
//                mapa->mapArray[j + n][i] = CAMINO;
//        }
//        if (n == 0)
//        {
//            //ir abajo
//            i++;
//            mapa->mapArray[j][i] = CAMINO;
//        }
        mapa->mapArray[j][i] = CAMINO;
        i++;
    }
    mapa->mapArray[j][i] = SALIDA;
//    for(j = 0; j < mapa->columnas; j++)
//    {
//        mapa->mapArray[i - 1][j] = PARED;
//        mapa->mapArray[j][i - 1] = PARED;
//    }
    return 1;
}

void mostrar_mapa(sMapa *mapa)
{
    int i, j;
    for (i = 0; i < mapa->filas; i++)
    {
        for (j = 0; j < mapa->columnas; j++)
        {
            if (mapa->mapArray[j][i] == 0)
                printf("_");
            else
                printf("%c", mapa->mapArray[j][i]);
        }
        printf("\n");
    }
}
