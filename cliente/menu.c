#include "menu.h"

//void menu_alta(SOCKET sock, char *request, char *response)
//{
//    char *nombre;
//    printf("Introduzca su nombre: ");
//    fgets(nombre, 6, stdin);
//    printf("\n");
//    request[0] = 'A'l
//    memcpy(request, nombre, 6);
//    alta_jugador(sock, request, response);
//}

void menu_derrota()
{
    char seleccion = 'P';
    system("cls");
    printf("Partida perdida\n");
    printf("Desea volver al menu principal? Y/N");
    do
    {
        scanf("%c", &seleccion);
    }while(seleccion != 'Y' && seleccion != 'N');
    if (seleccion == 'Y')
    {
//        mostrar_menu();
    }
    else
    {
        exit(0);
    }

}
