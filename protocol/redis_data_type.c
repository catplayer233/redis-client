//
// Created by mahongxu on 6/21/2021.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "redis_data_type.h"


char *num_as_str(_arg_num arg_num, char *str_buf) {
    sprintf(str_buf, "%d", arg_num);
    return str_buf;
}

char *make_command(char **command_args, _arg_num arg_num, char *command_buf) {
    //when you use the strcat or something like that you should make sure the dest string(char array) has enough spaces to keep the src str' chars
    //otherwise this will cause memory protection problem
    strcat(command_buf, "*");
    char arg_size_str[COMMAND_SIZE] = {0};
    strcat(command_buf, num_as_str(arg_num, arg_size_str));
    strcat(command_buf, TERMINATED_SYMBOL);
    for (_arg_num i = 0; i < arg_num; ++i) {
        char *arg = command_args[i];
        char byte_buf[COMMAND_SIZE] = {0};
        strcat(command_buf, "$");
        strcat(command_buf, num_as_str((_arg_num) strlen(arg), byte_buf));
        strcat(command_buf, TERMINATED_SYMBOL);
        strcat(command_buf, arg);
        strcat(command_buf, TERMINATED_SYMBOL);
    }
    return command_buf;
}