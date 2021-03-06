//
// Created by mahongxu on 6/23/2021.
//
#include <string.h>
#include "string_util.h"

#if defined(_WINDOWS) || defined(_WIN32)
#define strtok_r strtok_s
#endif

char *trim(char *str) {
    if (str == NULL)
        return NULL;
    //when the char is not the white space and not end of file
    char *head_addr = str;
    char *tail_addr = str + strlen(str) - 1;
    while (*head_addr <= ' ')
        head_addr++;
    while (*tail_addr <= ' ')
        tail_addr--;
    //not empty
    const size_t length = tail_addr + 1 - head_addr;
    if (length > 0) {
        *(tail_addr + 1) = '\0';
        return head_addr;
    } else
        return NULL;
}

char *split(char *origin_str, char *delimiter, char **cursor) {
    return strtok_r(origin_str, delimiter, cursor);
}