#pragma once

#include <arpa/inet.h>
#include <sys/time.h>

#define MESSAGE_LENGTH 10

typedef struct          s_socket
{
    int                 fd;
    struct sockaddr_in  addr;
    socklen_t           len;
    fd_set              active_set;
    fd_set              read_set;
}                       t_socket;

t_socket    *create_socket();
int         connect_client(t_socket *sok, const char *address, uint16_t port);
int         connect_server(t_socket *sok, uint16_t port);
void        destroy_socket(t_socket *client);
