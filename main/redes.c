#include "redes.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int auth_client(struct server_t* server, struct client_t* client) {

    return NOT_AUTHORIZED;
}

int accept_client(struct server_t* server,struct client_t* client) {

    client->raw_data.socklen = sizeof(struct sockaddr_in);

    for(int i = 0; i < connected_clients(server); i++) {
            if(server->connections[i]->m_ID == client->m_ID) {
                return NOT_AUTHORIZED; // retorna NOT_AUTHORIZED (0) quando o cliente já está conectado
            }
    }

    if(auth_client(server, client)) {

        

        return AUTHORIZED;
    }

    close(client->raw_data.file_descriptor);
    return NOT_AUTHORIZED; // retorna NOT_AUTHORIZED (0) quando não é possível criar um cliente adequadamente
}

int new_server(char* ip, uint16_t port, uint8_t protocol, pproc* process) {
    struct server_t* server = malloc(sizeof(struct server_t));
    if(!server) {
        return SOCKET_ERROR; // retorna SOCKET_ERROR (-1) quando não é possível alocar memória para o servidor
    }

    server->raw_data.bind_ip = ip;
    server->raw_data.bind_port = port;
    server->raw_data.domain = AF_INET;
    server->raw_data.type = SOCK_DGRAM;
    server->raw_data.protocol = protocol;

    server->raw_data.file_descriptor = socket(server->raw_data.domain, server->raw_data.type, server->raw_data.protocol);
    if(server->raw_data.file_descriptor < 0) {
        free(server);
        return SOCKET_ERROR; // retorna SOCKET_ERROR (-1) quando não é possível criar o socket
    }

    memset(&server->raw_data.raw, 0, sizeof(server->raw_data.raw));
    server->raw_data.raw.sin_family = server->raw_data.domain;
    server->raw_data.raw.sin_addr.s_addr = inet_addr(server->raw_data.bind_ip);
    server->raw_data.raw.sin_port = htons(server->raw_data.bind_port);

    if(bind(server->raw_data.file_descriptor, (struct sockaddr*)&server->raw_data.raw, sizeof(server->raw_data.raw)) < 0) {
        close(server->raw_data.file_descriptor);
        free(server);
        return ADDRESS_ERROR; // retorna ADDRESS_ERROR (-3) quando não é possível vincular o endereço ao socket
    }

    server->process = process;
    server->is_running = 1;
    return 1; // retorna 1 quando o servidor é criado com sucesso
}

void packet_handler(struct base_build* raw_data) {
    char buffer[MAX_MESSAGE_LENGTH];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    switch(read_packet(NULL, raw_data)) {
        case REQUEST:
            printf("Error reading packet from socket.\n");
            break;
        case SEND:
            printf("Timeout while waiting for packet.\n");
            break;
        case CONNECT:
            printf("Client connected.\n");
            break;
        case DISCONNECT:
            printf("Client disconnected.\n");
            break;
        default:
            // Process the received packet
            
            break;
    }
}





