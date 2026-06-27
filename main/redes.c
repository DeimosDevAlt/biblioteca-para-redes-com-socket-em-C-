#include "redes.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int auth_client(struct server_t* server, struct client_t* client) {
    (void)server;
    (void)client;
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

int new_server(struct server_t* server, char* ip, uint16_t port, uint8_t protocol, pproc process) {
    if(!server || !ip || !process) {
        return SOCKET_ERROR;
    }

    memset(server, 0, sizeof(*server));

    server->raw_data.bind_ip = malloc(strlen(ip) + 1);
    if(!server->raw_data.bind_ip) {
        return SOCKET_ERROR;
    }
    strcpy(server->raw_data.bind_ip, ip);

    server->raw_data.bind_port = port;
    server->raw_data.domain = AF_INET;
    server->raw_data.type = SOCK_DGRAM;
    server->raw_data.protocol = protocol;

    server->raw_data.file_descriptor = socket(server->raw_data.domain, server->raw_data.type, server->raw_data.protocol);
    if(server->raw_data.file_descriptor < 0) {
        free(server->raw_data.bind_ip);
        return SOCKET_ERROR;
    }

    memset(&server->raw_data.raw, 0, sizeof(server->raw_data.raw));
    server->raw_data.raw.sin_family = server->raw_data.domain;
    server->raw_data.raw.sin_port = htons(server->raw_data.bind_port);

    if(inet_pton(AF_INET, server->raw_data.bind_ip, &server->raw_data.raw.sin_addr) <= 0) {
        close(server->raw_data.file_descriptor);
        free(server->raw_data.bind_ip);
        return ADDRESS_ERROR;
    }

    if(bind(server->raw_data.file_descriptor, (struct sockaddr*)&server->raw_data.raw, sizeof(server->raw_data.raw)) < 0) {
        close(server->raw_data.file_descriptor);
        free(server->raw_data.bind_ip);
        return ADDRESS_ERROR;
    }

    server->process = process;
    server->is_running = 1;

    if(pthread_create(&server->tprocess, NULL, server->process, server) != 0) {
        close(server->raw_data.file_descriptor);
        free(server->raw_data.bind_ip);
        return CONNECTION_ERROR;
    }

    pthread_detach(server->tprocess);
    return 1;
}

int connected_clients(struct server_t* server) {
    (void)server;
    return 0;
}

int read_packet(struct packet_t* packet, struct base_build* raw_data) {
    
    packet->length = recvfrom(raw_data->file_descriptor, packet->data, packet->length, 0, (struct sockaddr*)&raw_data->raw, &raw_data->socklen);

    return REQUEST;
}

void setup_packet(struct packet_t* packet, char* buffer, uint8_t length, int client_ID, int flag) {
    packet->data = buffer;
    packet->length = length;
    packet->client_ID = client_ID;
    packet->flag = flag;
}

void packet_handler(struct base_build* raw_data) {
    char buffer[MAX_MESSAGE_LENGTH] = {0};
    struct sockaddr_in client_addr = {0};
    socklen_t addr_len = sizeof(client_addr);
    
    struct packet_t packet = {0};
    setup_packet(&packet, buffer, (uint8_t) MAX_MESSAGE_LENGTH, 0, 0);

    switch(read_packet(&packet, raw_data)) {
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





