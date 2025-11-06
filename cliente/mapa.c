#include "mapa.h"
//Generacion del mapa:
//Primer pasada
// Despues de generar las paredes que van a delimitar el mapa
// Elegir un punto aleatorio en la primer fila y marcarlo como entrada
// Bajar un nivel de ese punto
// De ah, elegir de manera aleatoria si ir hacia la izquierda o a la derecha y generar un camino en esa direccion
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

int cargar_mapa(sMapa *mapa, char *pathParametros)
{
    char bfStr[25];
    FILE *archParametros = fopen(pathParametros, "rb");
    fscanf(archParametros, "%s%i", bfStr, &mapa->filas);
    fscanf(archParametros, "%s%i", bfStr, &mapa->columnas);
    fscanf(archParametros, "%s%i", bfStr, &mapa->vidasInicio);
    fscanf(archParametros, "%s%i", bfStr, &mapa->maxFantasmas);
    fscanf(archParametros, "%s%i", bfStr, &mapa->maxPremios);
    fscanf(archParametros, "%s%i", bfStr, &mapa->maxVidasExtras);
    fclose(archParametros);
    return 1;
}

int generar_mapa(sMapa *mapa, tListaCircular *fantasmas)
{
    int i, j, n, entrada;
    srand(time(NULL));
    mapa->cantFantasmas = 0;
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
    mapa->entrada = entrada;
    mapa->mapArray[0][entrada] = ENTRADA;
    j = entrada;
    i = 1;

    //Generacion del camino principal, esto garantiza que haya una solucion al laberinto
    while(i < mapa->filas - 1)
    {
        n = aleatorio_entre(IZQ, DER);
        if (n == ABJ)
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
    // Para no recorrer dos veces el mapa y optimizar el codigo
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
                if (n >= 0 && n < 10 && mapa->cantFantasmas < mapa->maxFantasmas)
                {
                    coordenada nuevoFantasma;
                    mapa->mapArray[i][j] = FANTASMA;
                    mapa->maxFantasmas--;
                    nuevoFantasma.col = j;
                    nuevoFantasma.fil = i;
                    agregar_lista_circular(fantasmas, (void*)&nuevoFantasma, sizeof(nuevoFantasma));
                    mapa->cantFantasmas += 1;
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
    mapa->mapArray[1][entrada] = JUGADOR;
    return entrada;
}

void mostrar_mapa(sMapa *mapa)
{
    int i, j;
    for (i = 0; i < mapa->filas; i++)
    {
        for (j = 0; j < mapa->columnas; j++)
            printf("%c", mapa->mapArray[i][j]);
//        printf("% 2i", i);
        printf("\n");
    }
}

int guardar_mapa(sMapa *mapa, char *pathMapaTXT)
{
    int i = 0;
    char salto = '\n';
    FILE *mapaTXT = fopen(pathMapaTXT, "wt");
    for (i = 0; i < mapa->filas; i++)
    {
//        fwrite(mapa->mapArray[i], sizeof(char), mapa->columnas, mapaTXT);
//        fwrite(&salto, sizeof(char), 1, mapaTXT);
        if(fwrite(mapa->mapArray[i], sizeof(char), mapa->columnas, mapaTXT) < mapa->columnas || fwrite(&salto, sizeof(char), 1, mapaTXT) < 1)
            return 0;
    }
    fclose(mapaTXT);
    return 1;
}

void liberar_mapa(sMapa *mapa)
{
    int i;
    for (i = 0; i < mapa->filas; i++)
        free(mapa->mapArray[i]);
    free(mapa->mapArray);
}


int comparar_posiciones(void *coor1, void *coor2)
{
    coordenada c1 = *(coordenada*)coor1, c2 = *(coordenada*)coor2;
    return (c1.col == c2.col && c1.fil == c2.fil);
}

int handler(sMapa *mapa, jugador *personaje, tCola *movimientos, tListaCircular *fantasmas, tCola *movJugador)
{
    int res = 0, i = 0;
    movimiento movActual;
    coordenada fantasmaActual;
    //El primer movimiento siempre es el del jugador
    sacarDeCola(movimientos, (void*)&movActual, sizeof(movActual));
    switch(mapa->mapArray[movActual.posObjetivo.fil][movActual.posObjetivo.col])
    {
        case PARED:
        case ENTRADA:
            //do nothing
            break;
        case FANTASMA:
            personaje->vidas -= 1;
            if (personaje->vidas == 0)
            {
                res = LOSE;
                return res;
            }
            //Vuelve el jugador al inicio del juego
            personaje->col = mapa->entrada;
            personaje->fil = 1;
            mapa->mapArray[1][mapa->entrada] = JUGADOR;
            //Las celdas del fantasma y el jugador se dejan "vacias"
            mapa->mapArray[movActual.posOriginal.fil][movActual.posOriginal.col] = CAMINO;
            mapa->mapArray[movActual.posObjetivo.fil][movActual.posObjetivo.col] = CAMINO;

            //se reduce la cantidad en el mapa y se saca de la lista
            mapa->cantFantasmas -= 1;
            sacar_de_lista_valor(fantasmas, (void*)&(movActual.posObjetivo), comparar_posiciones);
            printf("Se elimina el fantasma en la pos [%i, %i]\n", movActual.posObjetivo.fil, movActual.posObjetivo.col);
            //El movimiento de los fantasmas se basan en la posicion del jugador, como ahora cambio ya no son validos, se vacian
            vaciarCola(movimientos);

            movActual.posOriginal = movActual.posObjetivo;
            movActual.posObjetivo.fil = 1;
            movActual.posObjetivo.col = mapa->entrada;
            ponerEnCola(movJugador, (void*)&movActual, sizeof(movActual));


            printf("Se vacio cola\n");
            break;
        case SALIDA:
            //you win!!
            res = WIN;
            break;
        case VIDA:
            personaje->vidas += 1;
            mapa->mapArray[movActual.posOriginal.fil][movActual.posOriginal.col] = CAMINO;
            mapa->mapArray[movActual.posObjetivo.fil][movActual.posObjetivo.col] = JUGADOR;
            personaje->fil = movActual.posObjetivo.fil;
            personaje->col = movActual.posObjetivo.col;
            break;
        case PREMIO:
            personaje->puntos += 1;
        case CAMINO:
            //normal
            mapa->mapArray[movActual.posOriginal.fil][movActual.posOriginal.col] = CAMINO;
            mapa->mapArray[movActual.posObjetivo.fil][movActual.posObjetivo.col] = JUGADOR;
            personaje->fil = movActual.posObjetivo.fil;
            personaje->col = movActual.posObjetivo.col;
            break;
    }


//        i = 0;
//        if (colaVacia(movimientos)== COLA_VACIA)
//        {
//            printf("No hay movimientos encolados.\n");
//        }
        while (colaVacia(movimientos) != COLA_VACIA)
        {
            ver_lista_circular(fantasmas, &fantasmaActual, sizeof(fantasmaActual));
            sacarDeCola(movimientos, &movActual, sizeof(movActual));
            switch(mapa->mapArray[movActual.posObjetivo.fil][movActual.posObjetivo.col])
            {
    //            case PARED:
    //            case ENTRADA:
    //            case FANTASMA:
    //            case SALIDA:
    ////            //do nothing
    ////                break;
                case JUGADOR:
    //                //die, go to start
                    personaje->vidas -= 1;
                    mapa->cantFantasmas -= 1;
                    if (personaje->vidas == 0)
                    {
                        res = LOSE;
                        vaciar_lista(fantasmas);
                        vaciarCola(movimientos);
                        return res;
                    }
                    sacar_de_lista_valor(fantasmas, (void*)&(fantasmaActual), comparar_posiciones);

                    mapa->mapArray[movActual.posOriginal.fil][movActual.posOriginal.col] = CAMINO;
                    mapa->mapArray[movActual.posObjetivo.fil][movActual.posObjetivo.col] = CAMINO;

                    personaje->col = mapa->entrada;
                    personaje->fil = 1;
                    mapa->mapArray[1][mapa->entrada] = JUGADOR;

                    vaciarCola(movimientos);
//                    for (i; i < mapa->cantFantasmas; i++)
//                    {
//                        avanzar_lista(fantasmas);
//                    }
                    movActual.posOriginal = movActual.posObjetivo;
                    movActual.posObjetivo.fil = 1;
                    movActual.posObjetivo.col = mapa->entrada;
                    ponerEnCola(movJugador, (void*)&movActual, sizeof(movActual));

//                    sacar_actual(fantasmas);
    //                sacar_de_lista_valor(fantasmas, movActual.posObjetivo, comparar_posiciones);
                    break;
                case CAMINO:
                    mapa->mapArray[movActual.posOriginal.fil][movActual.posOriginal.col] = CAMINO;
                    mapa->mapArray[movActual.posObjetivo.fil][movActual.posObjetivo.col] = FANTASMA;
                    fantasmaActual.col = movActual.posObjetivo.col;
                    fantasmaActual.fil = movActual.posObjetivo.fil;
                    sobreescribir_lista(fantasmas, &fantasmaActual, sizeof(fantasmaActual));
                    break;
            }
            i++;
            avanzar_lista(fantasmas);
        }

    return res;
}



int distancia_manhattan(coordenada a, coordenada b)
{
    int distancia;
    distancia = ABS(a.col - b.col) + ABS(a.fil - b.fil);
    return distancia;
}

coordenada movimiento_fantasma(coordenada posFantasma, coordenada posJugador, const sMapa *laberinto)
{
    coordenada direcciones[4];
    coordenada candidata;
    int i, distanciaMenor;
    for (i = 0; i < 4; i++)
    {
        direcciones[i].col = posFantasma.col;
        direcciones[i].fil = posFantasma.fil;
    }
    direcciones[0].col = posFantasma.col + 1;
    direcciones[1].col = posFantasma.col - 1;
    direcciones[2].fil = posFantasma.fil + 1;
    direcciones[3].fil = posFantasma.fil - 1;

    candidata = posFantasma;
    distanciaMenor = 1000000;
    for (i = 0; i < 4; i++)
    {
//        printf("La distancia para la celda [%i, %i] hacia [%i, %i] es %i\n", direcciones[i].fil, direcciones[i].col, posJugador.fil, posJugador.col, distancia_manhattan(direcciones[i], posJugador));
        if ((distancia_manhattan(direcciones[i], posJugador) < distanciaMenor ) && (laberinto->mapArray[direcciones[i].fil][direcciones[i].col] == CAMINO || laberinto->mapArray[direcciones[i].fil][direcciones[i].col] == JUGADOR))
        {
            candidata = direcciones[i];
            distanciaMenor = distancia_manhattan(direcciones[i], posJugador);
        }
    }
//    printf("El movimiento elegido para el fantasma en [%i, %i] es [%i, %i]\n", posFantasma.fil, posFantasma.col, candidata.fil, candidata.col);
    return candidata;
}

void juego(partida *nuevaPartida)
{
    sMapa mapa;
    jugador personaje;
    int res = 0, i, contMovimientos = 0;
    char entradaTeclado;
    movimiento movJugador, movFantasma;
    tCola movimientosEncolados, movimientosJugador;
    coordenada fantasmaActual, posJugador;

    crearCola(&movimientosEncolados);
    crearCola(&movimientosJugador);
    tListaCircular fantasmas;
    crear_lista(&fantasmas);

    if (cargar_mapa(&mapa, "config.txt") != 1)
    {
        printf("Error al cargar parametros.");
        exit(0);
    }
    printf("Parametros cargados.\n");
    if ((personaje.col = generar_mapa(&mapa, &fantasmas)) <= 1)
    {
        liberar_mapa(&mapa);
        printf("Error al generar el mapa.\n");
        exit(0);
    }
    printf("Mapa generado.");
    personaje.fil = 1;
    personaje.puntos = 0;
    personaje.vidas = mapa.vidasInicio;

    guardar_mapa(&mapa, "laberinto.txt");
    printf("Mapa guardado a txt.\n");
//    movJugador.posOriginal.col = personaje.col;
//    movJugador.posOriginal.fil = personaje.fil;
//    movJugador.posObjetivo = movJugador.posOriginal;
    while(res == 0)
    {
        system("cls");
        vaciarCola(&movimientosEncolados);
        posJugador.col = personaje.col;
        posJugador.fil = personaje.fil;
        movJugador.posOriginal.col = personaje.col;
        movJugador.posOriginal.fil = personaje.fil;
        movJugador.posObjetivo = movJugador.posOriginal;

        printf("VIDAS: %i | PUNTAJE: %i\n", personaje.vidas, personaje.puntos);

        mostrar_mapa(&mapa);
        entradaTeclado = getch();
        switch(entradaTeclado)
        {
            case ARRIBA:
                movJugador.posObjetivo.fil -= 1;
                break;
            case ABAJO:
                movJugador.posObjetivo.fil += 1;
                break;
            case DERECHA:
                movJugador.posObjetivo.col += 1;
                break;
            case IZQUIERDA:
                movJugador.posObjetivo.col -= 1;
                break;
        }
        printf("Movimiento: %c\n", entradaTeclado);
        contMovimientos++;
        ponerEnCola(&movimientosEncolados, (void*)&movJugador, sizeof(movJugador));
        ponerEnCola(&movimientosJugador, (void*)&movJugador, sizeof(movJugador));
        for (i = 0; i < mapa.cantFantasmas; i++)
        {
            ver_lista_circular(&fantasmas, (void*)&fantasmaActual, sizeof(fantasmaActual));
            movFantasma.posOriginal = fantasmaActual;
            movFantasma.posObjetivo = movimiento_fantasma(fantasmaActual, posJugador, &mapa);
            ponerEnCola(&movimientosEncolados, (void*)&movFantasma, sizeof(movFantasma));
            avanzar_lista(&fantasmas);
        }
        res = handler(&mapa, &personaje, &movimientosEncolados, &fantasmas, &movimientosJugador);

    }
    if (res == LOSE)
    {
//        system("cls");
        printf("Perdiste!!\n");
    }
    else if (res == WIN)
    {
        printf("Ganaste!!\n");
        //añadir partida y puntaje al servidor
    }
    nuevaPartida->movimientos = contMovimientos;
    nuevaPartida->puntaje = personaje.puntos;
    printf("Se mostrara el recorrido realizado por el jugador\n");
    system("pause");
    mostrar_movimientos(&mapa, &movimientosJugador);
//    printf("FIN DEL JUEGO\n");

}

void mostrar_movimientos(sMapa *mapa, tCola *movimientos)
{
    int i, j, cont = 0;
    movimiento movActual;
    printf("Se mostrará el recorrido realizado: \n");
    for (i = 1; i < mapa->filas - 1; i++)
    {
        for (j = 1; j < mapa->columnas; j++)
        {
            if (mapa->mapArray[i][j] != PARED)
            {
                mapa->mapArray[i][j] = CAMINO;
            }
        }
    }
    for (j = 0; j < mapa->columnas; j++)
    {
        if (mapa->mapArray[i-1][j] == JUGADOR)
        {
            mapa->mapArray[i-1][j] = CAMINO;
        }
    }
    mapa->mapArray[1][mapa->entrada] = JUGADOR;
    while (colaVacia(movimientos) != COLA_VACIA)
    {
        system("cls");
        cont++;
        sacarDeCola(movimientos, (void*)&movActual, sizeof(movActual));
        mostrar_mapa(mapa);
        Sleep(100);
//        system("pause");
//        printf("Presione cualquier tecla para seguir\n");
//        getch();
        if (mapa->mapArray[movActual.posObjetivo.fil][movActual.posObjetivo.col] != PARED)
        {
            mapa->mapArray[movActual.posOriginal.fil][movActual.posOriginal.col] = CAMINO;
            mapa->mapArray[movActual.posObjetivo.fil][movActual.posObjetivo.col] = JUGADOR;
        }
    }
    printf("La cantidad de movimientos realizados fue: %i\n", cont);
    printf("Presiones una tecla para volver al menu\n");
    system("pause");
}
