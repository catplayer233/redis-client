//
// Created by mahongxu on 6/23/2021.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "commands_utils.h"
#include "../string_util.h"


char *num_as_str(_arg_num arg_num, char *str_buf) {
    sprintf(str_buf, "%d", arg_num);
    return str_buf;
}

/*
 * check the target arguments is integer number or not.
 * if you want to pass an integer represent string you should add "" to declare the target argument is a string,
 * and we will remove the ""
 *
 * 0 means is not, use this as string, non-zero we treat the target string as integer number
 */
int check_integers(char **command_arg_ptr) {
    //the target command_arg already trim, no need to check the extra whitespaces
    char *command_arg = *command_arg_ptr;
    size_t arg_len = strlen(command_arg);
    //the arg has "", check the value
    if (*command_arg == '\"' && *(command_arg + arg_len) == '\"') {
        char *tmp_arg = calloc(arg_len - 1, sizeof(char));
        strncpy(tmp_arg, command_arg + 1, arg_len - 1);
        if (check_num(tmp_arg)) {
            //remove the ""
            *(command_arg + arg_len) = '\0';
            (*command_arg_ptr)++;
            free(tmp_arg);
            return TRUE;
        } else {
            free(tmp_arg);
            //keep the ""
            return FALSE;
        }
    }
    return check_num(command_arg);
}

char *make_command(char **command_args, _arg_num arg_num, char *command_buf) {
    //when you use the strcat or something like that you should make sure the dest string(char array) has enough spaces to keep the src str' chars
    //otherwise this will cause memory protection problem
    strcat(command_buf, type_prefix(ARRAYS));
    char arg_size_str[COMMAND_SIZE] = {0};
    strcat(command_buf, num_as_str(arg_num, arg_size_str));
    strcat(command_buf, TERMINATED_SYMBOL);
    for (_arg_num i = 0; i < arg_num; ++i) {
        char *arg = command_args[i];
        char byte_buf[COMMAND_SIZE] = {0};
        strcat(command_buf, type_prefix(BULK_STRINGS));
        strcat(command_buf, num_as_str((_arg_num) strlen(arg), byte_buf));
        strcat(command_buf, TERMINATED_SYMBOL);
        strcat(command_buf, arg);
        strcat(command_buf, TERMINATED_SYMBOL);
    }
    return command_buf;
}