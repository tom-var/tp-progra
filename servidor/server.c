#include "server.h"

int inicializar_winsock()
{
    WSADATA wsa; // Almacena informacion necesaria de la implementacion de sockets del sistema
    return WSAStartup(MAKEWORD(2, 2), &wsa); // Inicializa sockets con versión 2.2
}

SOCKET create_server_socket()
{
    //SOCKET = Tipo de dato
    //AF_INET = ipv4
    //SOCK_STREAM = El tipo de socket, en este caso, TCP.
    // 0 = Protocolo usado, al ser 0 se utiliza el protócolo por defecto para la combinación de ip y socket.
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
        return INVALID_SOCKET; //Devuelve esta macro si falló al crear el socket

    //sockaddr almacena información sobre la ip
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; //ipv4
    server_addr.sin_addr.s_addr = INADDR_ANY; //dirección ip, INADRR_ANY indica que puede ser cualquier interfaz local
    server_addr.sin_port = htons(PORT); // htons realiza una onversión del orden de bytes del sistema a orden de la red (big endian)

    //bind asocia un socket con una ip local, recibe el socket, la direccion almcenada en un sockaddr y el tamaño de dicha direccion
    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }
    // Pone el socket en "modo" escucha, esperando conexiones según el puerto asociado con bind.
    if (listen(s, 1) == SOCKET_ERROR) //El segundo parámetro indica cuántas conexiones permite, en este caso, solo una.
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    return s;
}

int comparar_registros(void *valor1, void *valor2)
{
    regIndice *aux1 = (regIndice*)valor1, *aux2 = (regIndice*)valor2;
    return strcmp(aux1->clave, aux2->clave);
}

//int alta_jugador(tArbolBinBusq *indice, char *nombre)
char alta_jugador(char *nombre)
{
    // Se verifica si ya existe el jugador.
    // Si no existe, se agrega a jugadores.dat y a indice.idx
    regIndice nuevoRegistro;
    FILE *archJugadores;
    jugador nuevoJugador;
    tIndice indice;
    int pos;

    crear_indice(&indice);
    cargar_indice(&indice, "indice.idx");
    strcpy(nuevoRegistro.clave, nombre);
    printf("El registro a buscar es %s\n", nuevoRegistro.clave);
    if((pos = buscar_indice(&indice, &nuevoRegistro, comparar_registros)) != -1)
    {
        printf("El jugador %s ya existe en pos %i.\n", nombre, pos);
        liberar_indice(&indice);
        return 'N';
    }
    printf("El jugador no existe\n");

    archJugadores = fopen("jugadores.dat", "a+b"); //Si existe lo abre al final, si no lo crea

    nuevoJugador.mejorPuntaje = 0;
    strcpy(nuevoJugador.nombre, nombre);
    nuevoJugador.partidasJugadas = 0;

    fseek(archJugadores, 0, SEEK_END);
    nuevoRegistro.posicion = ftell(archJugadores)/sizeof(jugador);

    fwrite(&nuevoJugador, sizeof(jugador), 1, archJugadores);
    fclose(archJugadores);

    insertar_indice(&indice, &nuevoRegistro, comparar_registros);
    guardar_indice(&indice, "indice.idx");
    liberar_indice(&indice);
    return 'Y';
}

//accion((*p)->info, (*p)->tamInfo, n, params);
void imprimir_registro(void *reg, unsigned tamDato, unsigned n, void * params)
{
    regIndice *registro = (regIndice*)reg;
    printf("%s | %i\n", registro->clave, registro->posicion);
}

int agregar_partida(char *partidaNueva)
{
    //Agrego partida al final del archivo
    partida *nuevaPartida = malloc(sizeof(partida));
    memcpy(nuevaPartida, partidaNueva, sizeof(partida));
    FILE *archPartidas = fopen("partidas.dat", "a+b");
    if (!archPartidas)
    {
        printf("Error en lectura de archivo");
        return -1;
    }
//    printf("se va a escribir de %s con %i puntos y %i movimientos\n", nuevaPartida->nombreJugador, nuevaPartida->puntaje, nuevaPartida->movimientos);
    fwrite(nuevaPartida, sizeof(partida), 1, archPartidas);
    fclose(archPartidas);

    //Cargo indice
    tIndice indice;
    crear_indice(&indice);
    cargar_indice(&indice, "indice.idx");
    //Busco al jugador en el indice para luego buscarlo en jugadores.dat y obtener la info
    jugador jugBuscado;
    regIndice regBuscado;
    strcpy(regBuscado.clave, nuevaPartida->nombreJugador);
    int pos =buscar_indice(&indice, &regBuscado, comparar_registros);

    FILE *archJugadores = fopen("jugadores.dat", "r+b");
    if (!archJugadores)
    {
        printf("Error en lectura de archivo");
        return -1;
    }
    fseek(archJugadores, pos*sizeof(jugador), SEEK_SET);
    fread(&jugBuscado, sizeof(jugador), 1, archJugadores);

    jugBuscado.partidasJugadas++;
    if(nuevaPartida->puntaje > jugBuscado.mejorPuntaje)
    {
        jugBuscado.mejorPuntaje = nuevaPartida->puntaje;
    }

    fseek(archJugadores, pos*sizeof(jugador), SEEK_SET);
    fwrite(&jugBuscado, sizeof(jugador), 1, archJugadores);
    fclose(archJugadores);

     printf("Se agrego partida de %s con %i puntos y %i movimientos\n", nuevaPartida->nombreJugador, nuevaPartida->puntaje, nuevaPartida->movimientos);
    return 1;
}

int comparar_jugadores(void *jugadorA, void *jugadorB)
{
    jugador *aux1 = (jugador*)jugadorA, *aux2 = (jugador*)jugadorB;
    return aux1->mejorPuntaje - aux2->mejorPuntaje;
}

int ranking(char *response, SOCKET client_socket)
{
    FILE *jugadores = fopen("jugadores.dat", "rb");
    jugador jugadorActual;
    tLista top;
    int n = 0;
    crear_lista(&top);

    fread(&jugadorActual, sizeof(jugador), 1, jugadores);
    poner_en_orden(&top, &jugadorActual, sizeof(jugador), comparar_jugadores);

    n++;
    while(fread(&jugadorActual, sizeof(jugador), 1, jugadores)>0)
    {
        if(poner_en_orden(&top, &jugadorActual, sizeof(jugador), comparar_jugadores) == 0)
        {
            printf("Error al insertar\n");
            return -1;
        }
        if (n < 10)
        {
            n++;
        }
        else
        {
            lista_eliminar_ultimo(&top);
        }
    }
    fclose(jugadores);

    imprimir_lista(&top);

    while(lista_vacia(&top) == NO_VACIA)
    {
        sacar_de_lista(&top, (void*)&jugadorActual, sizeof(jugadorActual));
        memcpy(response, &jugadorActual, sizeof(jugadorActual));
        send(client_socket, response, sizeof(jugadorActual), 0);
    }
    response[0] = '\0';
    send(client_socket, response, 1, 0);
//    printf("fin de top.\n");
    return 1;
}


void process_request(const char *request, char *response, SOCKET client_socket)
{
    char operation, text[BUFFER_SIZE];
    operation = request[0];


    if (operation == 'A')
    {

        strcpy(text, request + 1);
        printf("La operacion es %c y el parametro %s\n", operation, text);
        response[0] = alta_jugador(text);
        response[1] = '\0';
        printf("La respuesta fue %c\n", response[0]);
//        printf("Jugador %s dado de alta", text);
//        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (operation == 'P')
    {
        memcpy(text, request + 1, sizeof(partida));
        agregar_partida(text);
        response[0] = 'Y';
//        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (operation == 'R')
    {
        ranking(response, client_socket);
        response[0] = '\0';
    }
//    else
//    {
//        snprintf(response, BUFFER_SIZE, "Operacion no valida");
//    }
}

void run_server()
{
    if (inicializar_winsock() != 0)
    {
        printf("Error al inicializar Winsock\n");
        return;
    }

    SOCKET server_socket = create_server_socket();
    if (server_socket == INVALID_SOCKET)
    {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return;
    }

    printf("Servidor escuchando en puerto %d...\n", PORT);

    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);
    //Espera una conexin y retorna un nuevo socket especifico para esa conexion
    SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Error en accept()\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("Cliente conectado.\n");

    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int bytes_received;
    tCola requests;
    crearCola(&requests);

    /// recv tiene los siguientes parametros
    /// socket: socket conectado en uso (recibido por accept)
    /// buf: puntero a char donde esta la request en si (los datos recibidos)
    /// len: tamanio max en bytes del buffer
    /// flags: banderas, opciones

    ///Send actual practicamente igual, pero envia la informacion al cliente en vez de recibirla.


    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {

        buffer[bytes_received] = '\0';
        ponerEnCola(&requests, buffer, bytes_received + 1);
        printf("Recibido: %C\n", buffer[0]);
        sacarDeCola(&requests, buffer, bytes_received + 1);
        process_request(buffer, response, client_socket);
        send(client_socket, response, strlen(response), 0);
        printf("Enviado:  %c\n", response[0]);
    }
    vaciarCola(&requests);
    printf("Conexion cerrada.\n");
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
}
