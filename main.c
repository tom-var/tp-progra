#include "main.h"

int main()
{
    printf("Hello world!\n");
    sMapa miMapa;
    FILE *archConfig = fopen("config.txt", "rt");
    if (!archConfig)
        return 0;
    cargar_mapa(&miMapa, archConfig);
    fclose(archConfig);

    generar_mapa(&miMapa);
    mostrar_mapa(&miMapa);
    FILE *mapaArch = fopen("laberinto.txt", "wt");
    guardar_mapa(&miMapa, mapaArch);
    liberar_mapa(&miMapa);
    return 0;
}
