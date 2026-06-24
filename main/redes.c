#include "redes.h"

struct base_build {
    char bind_ip[15];
    u_int16_t bind_port;
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
    long uptime;
    struct client_t* connections;
};

struct packet_t {
    char* data;
    uint8_t length;
    int client_ID;
    int flag; // REQUEST, SEND, DISCONNECT
};




