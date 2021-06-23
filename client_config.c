//
// Created by mahongxu on 6/17/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string_util.h"
#include "client_config.h"

#define IP_VAR "host"
#define PORT_VAR "port"
#define USER_VAR "user"
#define PWD_VAR "password"
#define WAIT_TO "wait_timeout"
#define READ_TO "read_timeout"
#define MAX_LENGTH 256
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 6372
#define DEFAULT_WAIT_TO 60000
#define DEFAULT_READ_TO 60000

void take_property_value(client_config *client_config, FILE *config_file) {
    char buffer[MAX_LENGTH];
    char *target;
    //first argument bind a char buffer for keeping target string's content
    while ((target = trim(fgets(buffer, MAX_LENGTH, config_file))) != NULL) {
        //check first char, if the target is comment symbol # just ignore
        if (*target == '#' || *target == '=')
            continue;

        char *head = target;
        while (*target != '=')
            target++;
        const size_t key_len = target - head;
        char *key = malloc(key_len + 1);
        strncpy(key, head, key_len);
        *(key + key_len + 1) = '\0';
        if (strcmp(key, IP_VAR) == 0) {
            client_config->ip = target + 1;
        } else if (strcmp(key, PORT_VAR) == 0) {
            client_config->port = (port_int) atoi(target + 1);
        } else if (strcmp(key, USER_VAR) == 0) {
            client_config->user_name = target + 1;
        } else if (strcmp(key, PWD_VAR) == 0) {
            client_config->password = target + 1;
        } else if (strcmp(key, WAIT_TO) == 0) {
            client_config->wait_to = (time_second) atol(target + 1);
        } else if (strcmp(key, READ_TO) == 0) {
            client_config->read_to = (time_second) atol(target + 1);
        } else {
            //ignore
        }
    }
    //assert all client config
    if (client_config->ip == 0) {
        client_config->ip = DEFAULT_IP;
    }

    if (client_config->port == 0) {
        client_config->port = DEFAULT_PORT;
    }

    if (client_config->wait_to == 0) {
        client_config->wait_to = DEFAULT_WAIT_TO;
    }
    if (client_config->read_to == 0) {
        client_config->read_to = DEFAULT_READ_TO;
    }
}

client_config *load_client_config(char *file_name) {
    FILE *config_file = fopen(file_name, "r");
    if (config_file == NULL) {
        fprintf(stderr, "无法打开目标文件：%s\n", file_name);
        return NULL;
    }
    client_config *client_config_ptr = (client_config *) malloc(sizeof(client_config));
    take_property_value(client_config_ptr, config_file);
    //whatever success or not
    fclose(config_file);
    return client_config_ptr;
}
