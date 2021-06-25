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

void reply_output(char *command_reply) {
    char data_type = *command_reply;
    char *head = command_reply + 1;
    switch (data_type) {
        case INTEGERS:
        case SIMPLE_STRINGS:
            *(head + strlen(head) - 2) = '\0';
            printf("%s\n", head);
            break;
        case BULK_STRINGS: {
            int len_index = 0;
            //move forward util find the first \r
            while (*(head + len_index) != '\r') len_index++;
            if (len_index == 0) {
                printf("nil\n");
                return;
            }
            char *len_str = calloc(len_index, sizeof(char));
            strncpy(len_str, head, len_index);
            int str_len = atoi(len_str);
            free(len_str);
            if (str_len <= 0) {
                printf("nil\n");
                return;
            }
            *(head + len_index + str_len + 2) = '\0';
            printf("%s\n", head + len_index + 2);
            break;
        }
        case ERRORS:
            *(head + strlen(head) - 2) = '\0';
            fprintf(stderr, "%s\n", head);
            break;
        case ARRAYS: {
            //slice length obtain
            int slice_len_index = 0;
            while (*(head + slice_len_index) != '\r') slice_len_index++;
            if (slice_len_index == 0) {
                printf("nil\n");
                return;
            }
            char *len_str = calloc(slice_len_index, sizeof(char));
            strncpy(len_str, head, slice_len_index);
            int str_len = atoi(len_str);
            free(len_str);
            if (str_len <= 0) {
                printf("nil\n");
                return;
            }

            //apart every slice
            char *cursor = head + slice_len_index + 2;
            while (str_len > 0) {
                str_len--;
                char slice_data_type = *cursor;
                switch (slice_data_type) {
                    case INTEGERS:
                    case SIMPLE_STRINGS: {
                        int content_len = 0;
                        while (*(cursor + content_len) != '\r') content_len++;
                        char *slice_str = calloc(content_len + 2, sizeof(char));
                        strncpy(slice_str, cursor, content_len + 2);
                        reply_output(slice_str);
                        free(slice_str);
                        cursor += (content_len + 2);
                        break;
                    }
                    case BULK_STRINGS: {
                        int byte_len_index = 0;
                        char *bulk_head = cursor + 1;
                        while (*(bulk_head + byte_len_index) != '\r') byte_len_index++;
                        if (byte_len_index > 0) {
                            char *byte_len_str = calloc(byte_len_index, sizeof(char));
                            strncpy(byte_len_str, bulk_head, byte_len_index);
                            int byte_len = atoi(byte_len_str);
                            free(byte_len_str);

                            int bulk_len =
                                    byte_len > 0 ? ((1 + byte_len_index + 2) + (byte_len + 2))//(*xxx\r\n)+(content\r\n)
                                                 : (1 + byte_len_index + 2);//*-1\r\n
                            char *bulk_str = calloc(bulk_len, sizeof(char));
                            strncpy(bulk_str, cursor, bulk_len);
                            reply_output(bulk_str);
                            free(bulk_str);
                            cursor += bulk_len;
                        } else {
                            char *slice_str = calloc(4, sizeof(char));
                            strncpy(slice_str, cursor, 3);
                            reply_output(slice_str);
                            free(slice_str);
                            cursor += 3;
                        }
                        break;
                    }
                    default:
                        fprintf(stderr, "illegal data type in array: %c\n", slice_data_type);
                        return;
                }
            }
            break;
        }
        default:
            fprintf(stderr, "illegal reply: %s\n", command_reply);
    }
}