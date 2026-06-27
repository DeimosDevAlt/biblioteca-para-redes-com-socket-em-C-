#include "redes.h"

#include <stdio.h>
#include <stdlib.h>

void server_process(void *arg) {
    // Implement the server process logic here
    struct server_t* server = (struct server_t*)arg;
    while(server->is_running) {
        // Server logic to handle incoming connections and data
        packet_handler(&server->raw_data);
    }
}

void run_test(char* ip, uint16_t port) {

    printf("Starting server on %s:%d\n", ip, port);

    struct server_t* server = {0};
    int error_code = new_server(ip, port, 0, server_process);

    if(error_code < 0) {
        printf("Failed to create server. Error code: %d\n", error_code);
        return;
    }

    printf("Server is running\n");
    printf("Waiting for connections...\n");

    while(server->is_running) {
        // Server is running, you can add logic to handle server operations here
        

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
    run_test(LOCALHOST, 8080);
    return 0;
}