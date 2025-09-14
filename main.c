#include "main.h"

int main()
{
    tCola historico;
    tJugador jug;
    tLista ranking;

    crearCola(&historico);
    crearLista(&ranking);

    menu(&historico,&ranking);

    verPrimeroCola(&historico,&jug,sizeof(tJugador));
    printf("pos x: %d || pos y: %d || vidas final: %d || puntos final: %d",jug.movX,jug.movY,jug.cantVidas,jug.puntos);

    vaciarCola(&historico);
    vaciarLista(&ranking);

    return 0;
}
