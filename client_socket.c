#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "client_socket.h"
#include "string_util.h"

#ifdef __unix__

#include <sys/socket.h>
#include <netinet/in.h>

#define sock_startup()
#define sock_cleanup()

#elif defined(_WIN32) || defined(_WINDOWS)

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib") //Winsock Library

#define sock_startup() \
WSADATA wsa_data;\
int win_startup = WSAStartup(MAKEWORD(2, 2), &wsa_data);\
if (win_startup != 0) {\
printf("WSAStartup failed with error: %d\n", win_startup);\
exit(-3);\
}                       \

#define sock_cleanup() WSACleanup();

#define SHUT_RD 2

#endif

typedef struct sockaddr redis_addr;

unsigned int ip_literal2_int(char *literal_ip) {
    char *ip_slices[4] = {0};
    char *slice;
    int index = 0;
#if defined(_WINDOWS) || defined(_WINDOWS)//in windows we use stack allocated
    char *literal_ip_copy = "";
#else
    char *literal_ip_copy = malloc(sizeof(strlen(literal_ip) + 1)); //in linux we use heap allocated
#endif
    strcpy(literal_ip_copy, literal_ip);
#if defined(__unix__)
    char *head = literal_ip_copy;
#endif
    while ((slice = split(literal_ip_copy, ".", &literal_ip_copy)) != NULL)
        ip_slices[index++] = slice;
    unsigned int ip_int = 0;
    ip_int |= atoi(ip_slices[0]);
    ip_int |= (atoi(ip_slices[1]) << 8);
    ip_int |= (atoi(ip_slices[2]) << 16);
    ip_int |= (atoi(ip_slices[3]) << 24);
#if defined(__unix__)
    free(head);
#endif
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

    sock_startup()
    //create a socket
    const int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        fprintf(stderr, "can not create socket!\n");
        sock_cleanup()
        exit(-3);
    }

    redis_socket_ptr->socket_fd = socket_fd;

    //open connection
    redis_addr *server_addr = build_redis_addr(client_config);
    if (connect(socket_fd, server_addr, sizeof(redis_addr)) == -1) {
        fprintf(stderr, "connect redis server failed!\n");
        sock_cleanup()
        free(server_addr);
        exit(-3);
    }

    free(server_addr);

    //if client config set the password, we should do auth before execute any commands
    if (client_config->password != NULL) {
        //do auth
        char auth_command[COMMAND_SIZE] = {0};
        if (client_config->user_name != NULL) {
            auth_with_user(client_config->user_name, client_config->password, auth_command);
        } else {
            auth(client_config->password, auth_command);
        }

        if (send(socket_fd, auth_command, strlen(auth_command), 0) == -1) {
            fprintf(stderr, "auth command send failed!\n");
            disconnect_redis(redis_socket_ptr);
            exit(-3);
        }

        char auth_reply[REPLY_SIZE] = {0};
        //wait util get the result
        if (recv(socket_fd, auth_reply, REPLY_SIZE, 0) == -1) {
            fprintf(stderr, "no auth response returned!\n");
            disconnect_redis(redis_socket_ptr);
            exit(-3);
        }
        //show the result
        if (auth_reply[0] == ERRORS) {
            fprintf(stderr, "auth failed!\n");
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
        fprintf(stderr, "send command failed, try it later\n");
        return;
    }

    char command_reply[REPLY_SIZE] = {0};
    //wait util get the result
    if (recv(fd, command_reply, REPLY_SIZE, 0) == -1) {
        fprintf(stderr, "no respond from server\n");
        return;
    }
    printf("=> ");
    reply_output(command_reply);
    printf("---------------\n");
}

void disconnect_redis(redis_socket *redis_socket) {
    const int socket_fd = redis_socket->socket_fd;

    if (shutdown(socket_fd, SHUT_RD) == -1) {
        fprintf(stderr, "disconnect failed!\n");
        sock_cleanup()
        exit(-3);
    }
    sock_cleanup()
}
