#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "client_socket.h"

typedef struct sockaddr redis_addr;

unsigned int ip_literal2_int(char *literal_ip) {
    char *ip_slices[4] = {};
    char *slice;
    int index = 0;
    while ((slice = strtok_r(literal_ip, ".", &literal_ip)) != NULL)
        ip_slices[index++] = slice;

    unsigned int ip_int = 0;
    ip_int |= atoi(ip_slices[0]);
    ip_int |= (atoi(ip_slices[1]) << 8);
    ip_int |= (atoi(ip_slices[2]) << 16);
    ip_int |= (atoi(ip_slices[3]) << 24);

    return ip_int;
}

redis_addr *build_redis_addr(client_config *client_config) {
    struct sockaddr_in *server_addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(client_config->port);
    const struct in_addr ip = {.s_addr = ip_literal2_int(client_config->ip)};
    server_addr->sin_addr = ip;

    return (redis_addr *) server_addr;
}

redis_socket *create(client_config *client_config) {
    redis_socket *redis_socket_ptr = (redis_socket *) malloc(sizeof(redis_socket));
    redis_socket_ptr->config = client_config;
    //create a socket
    const int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1) {
        fprintf(stderr, "无法创建socket请稍后重试\n");
        exit(-3);
    }

    redis_socket_ptr->socket_fd = socket_fd;

    //open connection
    redis_addr *server_addr = build_redis_addr(client_config);
    if (connect(socket_fd, server_addr, sizeof(redis_addr)) == -1) {
        fprintf(stderr, "连接目标redis服务器失败！请检查相关参数!\n");
        exit(-3);
    }

    free(server_addr);

    //if client config set the password, we should do auth before execute any commands
    if (client_config->password != NULL) {
        //do auth
        char auth_command[COMMAND_SIZE] = {0};
        if (client_config->user_name != NULL)
            auth_with_user(client_config->user_name, client_config->password, auth_command);
        else
            auth(client_config->password, auth_command);

        if (send(socket_fd, auth_command, strlen(auth_command), 0) == -1) {
            fprintf(stderr, "认证失败，请重试！\n");
            disconnect_redis(redis_socket_ptr);
            exit(-3);
        }

        char auth_reply[REPLY_SIZE] = {0};
        //wait util get the result
        if (recv(socket_fd, auth_reply, REPLY_SIZE, 0) == -1) {
            fprintf(stderr, "认证失败，请重试！\n");
            disconnect_redis(redis_socket_ptr);
            exit(-3);
        }
        //show the result
        if (auth_reply[0] == ERRORS) {
            fprintf(stderr, "认证失败，请重试！\n");
            disconnect_redis(redis_socket_ptr);
            exit(-3);
        }
    }
    return redis_socket_ptr;
}

void execute_command(redis_socket *socket, char **command_args, _arg_num arg_num) {

    char command_buf[COMMAND_SIZE] = {0};
    char *command_str = make_command(command_args, arg_num, command_buf);
    const int fd = socket->socket_fd;
    //no need to free command_str, because the command_str is a pointer which point to the command_buf
    if (send(fd, command_str, strlen(command_str), 0) == -1) {
        printf("发送命令失败，请重试！\n");
        return;
    }

    char command_reply[REPLY_SIZE] = {0};
    //wait util get the result
    if (recv(fd, command_reply, REPLY_SIZE, 0) == -1) {
        printf("服务端无响应，请重试！\n");
        return;
    }
    printf("%s", command_reply);
}

void disconnect_redis(redis_socket *redis_socket) {
    const int socket_fd = redis_socket->socket_fd;

    if (shutdown(socket_fd, SHUT_RD) == -1) {
        fprintf(stderr, "断开与目标服务器的连接失败\n");
        exit(-3);
    }
}
