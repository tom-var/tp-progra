#include "mapa.h"
//Generación del mapa:
//Primer pasada
// Despues de generar las paredes que van a delimitar el mapa
// Elegir un punto aleatorio en la primer fila y marcarlo como entrada
// Bajar un nivel de ese punto
// De ahí, elegir de manera aleatoria si ir hacia la izquierda o a la derecha y generar un camino en esa dirección
// En cada paso hay una posibilidad de ir hacia abajo en vez de seguir. Si se llega a una pared, se va hacia abajo obligatoriamente.
// Repetir este proceso hasta llegar al "piso".
// Una vez se choca con el piso, marcarlo como salida.

//Segunda pasada
// Se recorre el mapa y se agrega camino o pared de manera aleatoria.

//Tercera pasada
// Se recorre el mapa y en los lugares donde hay camino se puebla con las entidades

int aleatorio_entre(int min, int max)
{
//    srand(time(NULL));
    return min + rand() % (max - min + 1);
}

int cargar_mapa(sMapa *mapa, FILE *archParametros)
{
    char bfStr[25];
    int bffr;
    fscanf(archParametros, "%s%i", bfStr, &mapa->filas);
    fscanf(archParametros, "%s%i", bfStr, &mapa->columnas);
    fscanf(archParametros, "%s%i", bfStr, &mapa->vidasInicio);
    fscanf(archParametros, "%s%i", bfStr, &mapa->maxFantasmas);
    fscanf(archParametros, "%s%i", bfStr, &mapa->maxPremios);
    fscanf(archParametros, "%s%i", bfStr, &mapa->maxVidasExtras);
    return 1;
}

int generar_mapa(sMapa *mapa)
{
    int i, j, n;
    srand(time(NULL));
    int entrada = aleatorio_entre(1, mapa->columnas - 2);

    //INICIALIZAR ARRAY DEL MAPA
    //TO DO: Validar que haya asignado memoria correctamente
    mapa->mapArray = (char**)malloc(mapa->columnas * sizeof(char*));
    for (i = 0; i < mapa->columnas; i++)
    {
        mapa->mapArray[i] = (char*)malloc(mapa->filas * sizeof(char));
    }

    //Generar paredes
    for(i = 0; i < mapa->columnas; i++)
    {
        mapa->mapArray[i][0] = PARED; // i = col j = fil
        mapa->mapArray[i][mapa->filas - 1] = PARED;
    }
    for(i = 0; i < mapa->filas; i++)
    {
        mapa->mapArray[0][i] = PARED;
        mapa->mapArray[mapa->columnas - 1][i] = PARED;
    }

    //Elegir entrada
    mapa->mapArray[entrada][0] = ENTRADA;
    mapa->mapArray[entrada][1] = CAMINO;
    j = entrada;
    i = 1;
    //Generacion del camino principal, esto garantiza que haya una solucion al laberinto
    while(i < mapa->filas - 1)
    {
        n = aleatorio_entre(IZQ, DER);
        if (n == ABAJO)
        {
            i++;
            mapa->mapArray[j][i] = CAMINO;
        }
        else
        {
            while(mapa->mapArray[j + n][i] != PARED && aleatorio_entre(0, 10) != 0) //Los parametros de la segunda condicion determinan la frecuencia en la que se "baja" antes de llegar a una pared
            {
                j += n;
                mapa->mapArray[j][i] = CAMINO;

            }
            i++;
            mapa->mapArray[j][i] = CAMINO;

        }
    }
    mapa->mapArray[j][i] = SALIDA;
    for (i = 1; i < mapa->columnas - 1; i++)
    {
        for (j = 1; j < mapa->filas - 1; j++)
        {
            if (mapa->mapArray[i][j] != '.')
            {
                n = aleatorio_entre(0,100);
                if (n >= 0 && n < 25)
                {
                    mapa->mapArray[i][j] = PARED;
                }
                else if (n >= 25 && n <= 100)
                {
                    mapa->mapArray[i][j] = CAMINO;
//                    if (n = aleatorio_entre(0, 20))
                }
            }
        }
    }
    for (i = 1; i < mapa->columnas - 1; i++)
    {
        for (j = 1; j < mapa->filas - 1; j++)
        {
            if (mapa->mapArray[i][j] == '.')
            {
                n = aleatorio_entre(0,100);
                if (n >= 0 && n < 10 && mapa->maxFantasmas > 0)
                {
                    mapa->mapArray[i][j] = FANTASMA;
                    mapa->maxFantasmas--;
                }
                if (n >= 10 && n < 20 && mapa->maxPremios > 0)
                {
                    mapa->mapArray[i][j] = PREMIO;
                    mapa->maxPremios--;
                }
                if (n >= 20 && n < 30 && mapa->maxVidasExtras > 0)
                {
                    mapa->mapArray[i][j] = VIDA;
                    mapa->maxVidasExtras--;
                }
            }
        }
    }
    return 1;
}

void mostrar_mapa(sMapa *mapa)
{
    int i, j;
    for (i = 0; i < mapa->filas; i++)
    {
        for (j = 0; j < mapa->columnas; j++)
            printf("%c", mapa->mapArray[j][i]);
        printf("\n");
    }
}
