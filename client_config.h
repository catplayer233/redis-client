//
// Created by mahongxu on 6/17/2021.
//

#ifndef REDIS_CLIENT_CLIENTCONFIG_H
#define REDIS_CLIENT_CLIENTCONFIG_H

#endif //REDIS_CLIENT_CLIENTCONFIG_H

typedef unsigned short int port_int;
typedef unsigned int time_second;

typedef struct client_config {
    port_int port;
    time_second wait_to, read_to;
    char *ip;
    char *user_name;
    char *password;
} client_config;

client_config *load_client_config(char *);
