//
// Created by mahongxu on 6/21/2021.
//
#ifndef REDIS_CLIENT_CLIENT_SOCKET_H
#define REDIS_CLIENT_CLIENT_SOCKET_H

#endif //REDIS_CLIENT_CLIENT_SOCKET_H

#include "client_config.h"
#include "command/util_commands.h"

#define REPLY_SIZE  2048

typedef struct redis_socket {
    client_config *config;
    int socket_fd;
} redis_socket;

redis_socket *create(client_config *);

void disconnect_redis(redis_socket *);

void execute_command(redis_socket *, char **, _arg_num);
