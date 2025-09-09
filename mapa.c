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

int aleatorio_entre(int min, int max) // Posible optimizacion pasar a una macro?
{
    return min + rand() % (max - min + 1);
}

int cargar_mapa(sMapa *mapa, FILE *archParametros)
{
    char bfStr[25];
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
    int i, j, n, entrada;
    srand(time(NULL));

    //INICIALIZAR ARRAY DEL MAPA
    mapa->mapArray = (char**)malloc(mapa->filas * sizeof(char*));
    if (mapa->mapArray == NULL)
    {
        return SIN_MEM;
    }
    for (i = 0; i < mapa->filas; i++)
    {
        mapa->mapArray[i] = (char*)malloc(mapa->columnas * sizeof(char));
        if(mapa->mapArray[i] == NULL)
        {
            for (; i > 0; i--)
            {
                free(mapa->mapArray[i]);
            }
        free(mapa->mapArray);
        return SIN_MEM;
        }
    }

    //Generar paredes
    for(i = 0; i < mapa->filas; i++)
    {
        mapa->mapArray[i][0] = PARED; // i = col j = fil
        mapa->mapArray[i][mapa->columnas - 1] = PARED;
    }
    for(i = 0; i < mapa->columnas; i++)
    {
        mapa->mapArray[0][i] = PARED;
        mapa->mapArray[mapa->filas - 1][i] = PARED;
    }

    //Marcar entrada y salida
    entrada = aleatorio_entre(1, mapa->columnas - 2);
    mapa->mapArray[0][entrada] = ENTRADA;
    mapa->mapArray[1][entrada] = CAMINO;
    j = entrada;
    i = 1;

    //Generacion del camino principal, esto garantiza que haya una solucion al laberinto
    while(i < mapa->filas - 1)
    {
        n = aleatorio_entre(IZQ, DER);
        if (n == ABAJO)
        {
            i++;
            mapa->mapArray[i][j] = CAMINO;
        }
        else
        {
            while(mapa->mapArray[i][j+n] != PARED && aleatorio_entre(0, 10) != 1) //Los parametros de la segunda condicion determinan la frecuencia en la que se "baja" antes de llegar a una pared
            {
                j += n;
                mapa->mapArray[i][j] = CAMINO;
            }
            i++;
            mapa->mapArray[i][j] = CAMINO;
        }
    }
    mapa->mapArray[i][j] = SALIDA;

    //Se recorre el mapa y se llena con paredes y camino
    //Se puede modificar las posibilidades para variar la distribucion de paredes y caminos
    for (i = 1; i < mapa->filas - 1; i++)
    {
        for (j = 1; j < mapa->columnas - 1; j++)
        {
            if (mapa->mapArray[i][j] != '.') //chequea si no es parte del camino principal
            {
                n = aleatorio_entre(0,100);
                if (n >= 0 && n < 25)
                {
                    mapa->mapArray[i][j] = PARED;
                }
                else if (n >= 25 && n <= 100)
                {
                    mapa->mapArray[i][j] = CAMINO;
                }
            }
        }
    }
    //Se recorre el mapa y esta vez se habitan los caminos con las entidades
    //Posible mejora
    // Para no recorrer dos veces el mapa y optimizar el código
    // Combinar este paso con el anterior
    // En el loop anterior cuando se ponga una pared
    // Que tambien haya una posibilidad de poner una entidad
    for (i = 1; i < mapa->filas - 1; i++)
    {
        for (j = 1; j < mapa->columnas - 1; j++)
        {
            if (mapa->mapArray[i][j] == '.')
            {
                //Posible mejora
                // Se suma columna*filas para que a medida que crece el mapa
                // Se distribuyan mejor las entidades
                // De lo contrario tienden a colocarse al principio del mapa
                // Es una solucion un poco rudimentaria, quizas haya una mejor manera
                n = aleatorio_entre(0,100+mapa->columnas*mapa->filas);
                if (n >= 0 && n < 10 && mapa->maxFantasmas > 0)
                {
                    mapa->mapArray[i][j] = FANTASMA;
                    mapa->maxFantasmas--;
                }
                else if (n >= 10 && n < 20 && mapa->maxPremios > 0)
                {
                    mapa->mapArray[i][j] = PREMIO;
                    mapa->maxPremios--;
                }
                else if (n >= 20 && n < 30 && mapa->maxVidasExtras > 0)
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
            printf("%c", mapa->mapArray[i][j]);
        printf("\n");
    }
}

int guardar_mapa(sMapa *mapa, FILE *mapaTXT)
{
    int i = 0;
    char salto = '\n';
    for (i = 0; i < mapa->filas; i++)
    {
//        fwrite(mapa->mapArray[i], sizeof(char), mapa->columnas, mapaTXT);
//        fwrite(&salto, sizeof(char), 1, mapaTXT);
        if(fwrite(mapa->mapArray[i], sizeof(char), mapa->columnas, mapaTXT) < mapa->columnas || fwrite(&salto, sizeof(char), 1, mapaTXT) < 1)
            return 0;
    }
    return 1;
}

void liberar_mapa(sMapa *mapa)
{
    int i;
    for (i = 0; i < mapa->filas; i++)
        free(mapa->mapArray[i]);
    free(mapa->mapArray);
}


int mover_entidad(int *fil, int *col, char direccion, sMapa *mapa, int *posInicialY)
{

    int x = 0, y = 0;
    printf("DIRECCION = %i\n", direccion);
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
    else if (direccion == ABAJO)
    {
        x = 1;
        y = 0;
    }
    printf("x: %i, y: %i\n", x, y);

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
