#include "main.h"

int main()
{
    sMapa miMapa;

    char eleccion,e;
    int res,puntos=0;
    int posX, posY,iniY;

    tCola historico;
    crearCola(&historico);
    
    FILE *archConfig = fopen("config.txt", "rt");
    if (!archConfig)
        return 0;
    FILE *mapaArch = fopen("laberinto.txt", "wt");
    if(!mapaArch)
        return 0;
    
    posX = 1;
    posY = generar_mapa(&miMapa);
    iniY=posY;

    cargar_mapa(&miMapa, archConfig);
    fclose(archConfig);
    
    while(TOUP(eleccion) != 'P')
    {
        printf("x: %i, y: %i\n", posX, posY);
        printf("Cantidad de vidas:  %u \n",miMapa.vidasInicio);
        printf("Puntos: %d \n",puntos);

        mostrar_mapa(&miMapa);

        scanf("%c", &eleccion);
        fflush(stdin);
        eleccion=TOUP(eleccion);

        //aca queria ver si podia hacer lo del registro de movimientos
        ponerEnCola(&historico,&eleccion,sizeof(char));

        res = mover_entidad(&posX, &posY, TOUP(eleccion), &miMapa,&iniY);
        if (res == LOSE && miMapa.vidasInicio==1)
        {
            printf("Perdiste!!\n");
            return 0;
        }
        else if(res==LOSE)
            miMapa.vidasInicio--; //si toca fantasma, resto vida

        if(res==HP)
            miMapa.vidasInicio++; //si toca vida, sumo

        if (res == WIN)
        {
            printf("WIN!!\n");
            return 0;
        }
        if(res==POINT)
            puntos+=1; //si toca puntos, sumo

        printf("\n");
        system("cls");
    }

    //aca queria ver si podia hacer lo del registro de movimientos
    verPrimeroCola(&historico,&e,sizeof(eleccion));
    printf("Primer movimiento: %c",e);
    
    guardar_mapa(&miMapa, mapaArch);
    liberar_mapa(&miMapa);
    vaciarCola(&historico);
    fclose(mapaArch);
    return 0;
}
