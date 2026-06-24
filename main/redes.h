#include <arpa/inet.h>
#include <sys/socket.h>

struct base_build {
    char bind_ip[15];
    u_int16_t bind_port;
    int domain;
    int type;
    int protocol;
};

struct client {
    struct base_build raw_data; 
};