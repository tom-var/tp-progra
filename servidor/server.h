#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <winsock2.h>
#include <string.h>
#include "structs.h"
#include "arbol.h"
#include "indice.h"
#include "lista.h"
#include "cola.h"

#define PORT 12345
#define BUFFER_SIZE 1024

int inicializar_winsock();

SOCKET create_server_socket();

void process_request(const char *request, char *response, SOCKET client_socket);

void run_server();

void imprimir_registro(void *reg, unsigned tamDato, unsigned n, void * params);



//funciones de juego

int buscar_jugador();

char alta_jugador();





#endif // SERVER_H_INCLUDED
