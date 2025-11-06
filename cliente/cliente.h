#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024
#include "estructuras.h"

typedef struct
{
    char nombre[6];
    unsigned    mejorPuntaje,
                partidasJugadas;
}jugadorRanking;


// Inicializa Winsock
int init_winsock();

// Conecta al servidor y devuelve el socket
SOCKET connect_to_server(const char *server_ip, int port);

// Envia una solicitud y recibe la respuesta
int send_request(SOCKET sock, const char *request, char *response);

// Cierra la conexion y limpia Winsock
void close_connection(SOCKET sock);

void ranking(SOCKET sock, const char *request, char *response);
void alta_jugador(SOCKET sock, const char *request, char *response);
void agregar_partida(SOCKET sock, const char *request, char *response);


#endif // CLIENTE_H_INCLUDED
