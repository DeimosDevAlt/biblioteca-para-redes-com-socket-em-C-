#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdint.h>

#define LOCALHOST "127.0.0.1"
#define REQUEST 0
#define SEND 1
#define CONNECT 2
#define DISCONNECT 3
#define SOCKET_ERROR -1
#define CONNECTION_ERROR -2
#define ADDRESS_ERROR -3
#define AUTHORIZED 1
#define NOT_AUTHORIZED 0
#define TIMEOUT 2
#define MAX_MESSAGE_LENGTH 512

typedef pthread_t (*pproc) (void*);

struct base_build {
    char* bind_ip;
    uint16_t bind_port;
    struct sockaddr_in raw;
    socklen_t socklen;
    int file_descriptor;
    int domain;
    int type;
    int protocol;
};

struct client_t {
    struct base_build m_raw_data;
    struct base_build raw_data;
    pproc* process;
    long uptime;
    int m_ID;
};

struct server_t {
    struct base_build raw_data;
    pproc* process;
    int is_running;
    long uptime;
    struct client_t** connections;
};

struct packet_t {
    char* data;
    uint8_t length;
    int client_ID;
    int flag;
};

int auth_client(struct server_t*, struct client_t*);

int accept_client(struct server_t*, struct client_t*);

void run_client(struct client_t, pproc*);

int new_server(char*, uint16_t, uint8_t, pproc*);

int new_client(char*, uint16_t, uint8_t, pproc*);

struct client_t get_client_by_ID(struct server_t, int);

int connected_clients(struct server_t*);

int is_connected(struct client_t);

int disconnect_client(struct client_t);

int disconnect_client_by_ID(struct server_t, int);

int shutoff_server(struct server_t);

void packet_handler(struct base_build*);

void clean_packet(struct packet_t*);

int read_packet(struct packet_t*, struct base_build*);

void send_packet(struct packet_t*, struct base_build*);

void send_to_all_clients(struct packet_t*, struct server_t*);

void send_to_client(struct packet_t*, struct client_t*);

void send_to_all_except(struct packet_t*, struct server_t*, int);
