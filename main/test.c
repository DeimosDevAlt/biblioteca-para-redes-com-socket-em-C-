#include "redes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* server_process(void *arg) {
    struct server_t* server = (struct server_t*)arg;
    fflush(stdout);
    printf("debug");
    while(server->is_running) {
        packet_handler(&server->raw_data);
    }
    return NULL;
}

void run_test(char* ip, uint16_t port) {

    printf("Starting server on %s:%d\n", ip, port);

    struct server_t* server = malloc(sizeof(struct server_t));
    *server = (struct server_t){0}; // Initialize the server structure
    int error_code = new_server(server, ip, port, 0, server_process);

    if((error_code != 1)) {
        printf("Failed to create server. Error code: %d\n", error_code);
        return;
    }

    printf("Server is running\n");
    printf("Waiting for connections...\n");

    while(server->is_running) {
        // Server is running, you can add logic to handle server operations here
        char server_commands[32] = {0};
        scanf("%s", server_commands);

        if(strcmp(server_commands, "stop") == 0) {
            server->is_running = 0;
        }

    }

    switch(error_code) { 
        case SOCKET_ERROR:
            printf("Error creating server socket.\n");
            break;
        case CONNECTION_ERROR:
            printf("Error accepting client connection.\n");
            break;
        case ADDRESS_ERROR:
            printf("Error binding server to address.\n");
            break;
        default:
            break;
    }

    printf("Server stopped running.\n");
}

int main() {
    run_test(LOCALHOST, 8081);
    return 0;
}