#include "menu.h"

void menu(tCola *historico,tLista *lista)
{
    int opcion;
    tJugador jugador;
    char alias[10];

    while(opcion!=3)
    {
        puts("------MENU-----");
        puts("1. Jugar nueva partida");
        puts("2. Ver el ranking de jugadores");
        puts("3. Salir del juego\n");

        do{
            puts("Ingrese opcion: ");
            scanf("%d",&opcion);
        }while(opcion<1 && opcion>3);

        if(opcion==1)
        {
            printf("Ingrese un alias: ");
            scanf("%s",alias);
            system("cls");
            strcpy(jugador.alias,alias);

            nuevapartida(historico,&jugador);
            ponerEnListaOrdenado(lista,&jugador,sizeof(tJugador),1,comparaEstructura);
        }else if(opcion==2){
            if(listaVacia(lista)==LISTA_VACIA)
            {
                puts("Ranking de jugadores vacio");
            }
            else
            {
                puts("----LISTADO DE JUGADORES---");
                puts("---ALIAS----PUNTAJE---");
                mostrarLista(lista,mostrarEstructura);
            }
        }
    }

}

int nuevapartida(tCola *historico,tJugador *jugador)
{
    char eleccion;
    int res;
    int posX, posY,iniY;

    sMapa miMapa;

    FILE *archConfig = fopen("config.txt", "rt");
    if (!archConfig)
        return 0;

    cargar_mapa(&miMapa, archConfig);
    fclose(archConfig);

    jugador->puntos=0;
    jugador->cantVidas=(int)miMapa.vidasInicio;

    posX = 1;
    posY = generar_mapa(&miMapa);
    iniY=posY;

    FILE *mapaArch = fopen("laberinto.txt", "wt");

    while(TOUP(eleccion) != 'P')
    {
        printf("x: %i, y: %i\n", posX, posY);
        printf("Cantidad de vidas:  %d \n",jugador->cantVidas);
        printf("Puntos: %d \n",jugador->puntos);

        mostrar_mapa(&miMapa);

        scanf("%c", &eleccion);
        fflush(stdin);
        eleccion=TOUP(eleccion);

        res = mover_entidad(&posX, &posY, TOUP(eleccion), &miMapa,&iniY);
        if (res == LOSE && jugador->cantVidas==1)
        {
            printf("Perdiste!!\n");
            return 0;
        }
        else if(res==LOSE)
            jugador->cantVidas--; //si toca fantasma, resto vida

        if(res==HP)
            jugador->cantVidas++; //si toca vida, sumo

        if (res == WIN)
        {
            printf("WIN!!\n");
            return 0;
        }
        if(res==POINT)
            jugador->puntos+=1; //si toca puntos, sumo


        jugador->movX=posX;
        jugador->movY=posY;

        ponerEnCola(historico,jugador,sizeof(tJugador));

        printf("\n");
        system("cls");
    }

    guardar_mapa(&miMapa, mapaArch);
    liberar_mapa(&miMapa);


    fclose(mapaArch);

    return TODO_OK;
}


int comparaEstructura(const void *a,const void *b)
{
    tJugador *auxA=(tJugador*)a,*auxB=(tJugador*)b;
    int comp;

    comp=strcmp(auxA->alias,auxB->alias);

    if(comp<0 || comp>0)
        return comp;

    if(auxA->puntos!=auxB->puntos)
        return auxA->puntos-auxB->puntos;

    return TODO_OK;

}

void mostrarEstructura(const void *a)
{
    tJugador *jugador=(tJugador*)a;

    printf("\t%s \t %d \n",jugador->alias,jugador->puntos);
}
