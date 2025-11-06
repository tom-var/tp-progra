#include "cliente.h"
#include <stdio.h>
#include <string.h>

int init_winsock() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET connect_to_server(const char *server_ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

int send_request(SOCKET sock, const char *request, char *response) {
    if (send(sock, request, strlen(request), 0) < 0) {
        return -1;
    }

    int bytes_received = recv(sock, response, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        return -1;
    }

    response[bytes_received] = '\0';
    return 0;
}

void close_connection(SOCKET sock) {
    closesocket(sock);
    WSACleanup();
}

void ranking(SOCKET sock, const char *request, char *response)
{
    int cont = 0, bytesRecibidos;
    jugadorRanking *aux;
    if (send(sock, request, strlen(request), 0) < 0)
    {
        printf("No se pudo conectar con el servidor o hubo un error en la conexión.\n");
        return;
    }
    printf("POS\tNOMBRE \t PUNTUACION\n");
//    while((bytesRecibidos = recv(sock, response, sizeof(jugadorRanking), 0)) > 0 && response[0] != '\0')
    bytesRecibidos = recv(sock, response, sizeof(jugadorRanking), 0);
    while(response[0] != '\0')
    {
        aux = (jugadorRanking*)response;
        printf("%i)\t%s \t %i\n", cont, aux->nombre, aux->mejorPuntaje);
        cont++;
        bytesRecibidos = recv(sock, response, sizeof(jugadorRanking), 0);
    }
    printf("--- FIN DEL RANKING ---\n");
    system("pause");
}

void alta_jugador(SOCKET sock, const char *request, char *response)
{
    int bytesRecibidos;
    if (send(sock, request, strlen(request), 0) < 0)
    {
        printf("No se pudo conectar con el servidor o hubo un error en la conexión.\n");
        return;
    }

    if ((bytesRecibidos = recv(sock, response, BUFFER_SIZE, 0)) > 0)
    {
        if (response[0] == 'Y') //Ya existe
        {
            printf("El jugador  ya existe. La partida se agregara a su listado.\n");
        }
        else if (response[0] == 'N')
        {
            printf("Se agregó jugador\n");
        }
    }
}

void agregar_partida(SOCKET sock, const char *request, char *response)
{
    int bytesRecibidos;
    partida nuevaPartida;
    memcpy(&nuevaPartida, request + 1, sizeof(nuevaPartida));
//    printf("El request es %c para %s con puntos %i y moviomientos %i\n", request[0], newParty.nombreJugador, newParty.puntaje, newParty.movimientos);

    if (send(sock, request, sizeof(partida) + 1, 0) < 0)
    {
        printf("No se pudo conectar con el servidor o hubo un error en la conexión.\n");
        return;
    }

    if ((bytesRecibidos = recv(sock, response, BUFFER_SIZE, 0)) > 0)
    {

        if (response[0] == 'Y') //Ya existe
        {
            printf("Se agrego una nueva partida.\n");
        }
        else if (response[0] == 'N')
        {
            printf("Hubo un error al querer agregar la partida\n");
        }
        else
        {
            return;
        }
    }
}
