#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define LOCALHOST "127.0.0.1"
#define REQUEST 0
#define SEND 1
#define DISCONNECT 2

typedef pthread_t (*pproc) (int);

struct base_build;
struct client_t;
struct server_t;
struct packet_t;

int new_client(char*, uint16_t);

void run_client(struct client_t, pproc*);

int new_server(char*, uint16_t, uint8_t, pproc*);

struct client_t get_client_by_ID(struct server_t, int);

int connected_clients(struct server_t);

int is_connected(struct client_t);

int disconnect_client(struct client_t);

int disconnect_client_by_ID(struct server_t, int);

int shutoff_server(struct server_t);

void clean_packet(struct packet_t*);
