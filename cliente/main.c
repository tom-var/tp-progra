#include "main.h"
#include <conio.h>
#include "lista.h"

int main()
{
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    if (init_winsock() != 0) {
        printf("Error al inicializar Winsock\n");
        return 1;
    }

    SOCKET sock = connect_to_server(SERVER_IP, PORT);
    if (sock == INVALID_SOCKET)
        {
        printf("No se pudo conectar al servidor\n");
        printf("No se podra ver ranking ni guardar partidas.\n");
        WSACleanup();
    }


    printf("----FANTASMAS Y LABERINTOS----\n");

    char seleccion = 'P';
    while (1)
    {
        system("cls");
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("A) Jugar\n");
        printf("B) Ver ranking\n");
        printf("C) Salir\n");
        printf("Seleccione una opcion: ");

        scanf("%c", &seleccion);
        fflush(stdin);

        switch (seleccion)
        {
            case 'A':
            {
                char nombre[10];
                printf("Introduzca su nombre: ");
                fgets(nombre, 10, stdin);
                nombre[strcspn(nombre, "\n")] = '\0';

                buffer[0] = 'A';
                memcpy(buffer + 1, nombre, 10);
                printf("Nombre introducido: %s\n", nombre);
                alta_jugador(sock, buffer, response);

                partida nuevaPartida;
                strcpy(nuevaPartida.nombreJugador, nombre);
                juego(&nuevaPartida);

                buffer[0] = 'P';
                memcpy(buffer + 1, (void*)&nuevaPartida, sizeof(nuevaPartida));
                printf("Se enviara al servidor partida de %s con %i puntos y %i movimientos\n",
                nuevaPartida.nombreJugador, nuevaPartida.puntaje, nuevaPartida.movimientos);
                agregar_partida(sock, buffer, response);

                break;
            }
            case 'B':
            {
                printf("Solicitando ranking...\n");
                buffer[0] = 'R';
                ranking(sock, buffer, response);
                break;
            }
            case 'C':
            {
                printf("Saliendo...\n");
                close_connection(sock);
                exit(0);
            }
            default:
            {
                if (seleccion != '\n') {
                    printf("Opción no válida. Por favor, intente de nuevo.\n");
                }
                break;
            }
        }
    }


    return 0;
}
