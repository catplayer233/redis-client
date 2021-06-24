#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_util.h"
#include "command/commands_utils.h"
#include "client_socket.h"

void assert_env(const int argc, char **argv) {
    if (argc <= 1) {
        fprintf(stderr, "若要使用该程序，请先指定redis连接环境的配置文件，如client.conf\n");
        exit(-1);
    }

    if (argc > 2) {
        fprintf(stderr, "您只需要指定一个配置文件路径的参数即可，请不要指定过多无用参数\n");
        exit(-1);
    }
}

int main(const int argc, char **argv) {
    assert_env(argc, argv);
    client_config *client_config = load_client_config(argv[1]);
    if (client_config == NULL) exit(-2);
    printf("redis服务器=> IP: %s, 端口: %d\n", client_config->ip, client_config->port);
    redis_socket *redis_socket = create(client_config);
    printf("您已成功建立与服务器的连接，现在进入交互模式，你可以发送redis命令，输入q+回车，结束\n");

    char input_buf[COMMAND_SIZE];
    char *command_buf;
    while (strcmp((command_buf = trim(fgets(input_buf, COMMAND_SIZE, stdin))), "q") != 0) {
        //max size set 8
        char *command_args[8];
        char *command_arg;
        _arg_num arg_index = 0;
        while ((command_arg = strtok_r(command_buf, " ", &command_buf)) != NULL)
            command_args[arg_index++] = command_arg;
        execute_command(redis_socket, command_args, arg_index);
    }
    disconnect_redis(redis_socket);
    free(redis_socket);
    free(client_config);
    return 0;
}

/*
 * test purpose
 */
//int main() {
//    char command_buf[1024] = {0};
//    printf("%s\n", auth("123", command_buf));
//    return 0;
//}