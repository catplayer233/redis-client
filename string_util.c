//
// Created by mahongxu on 6/23/2021.
//
#include <string.h>
#include "string_util.h"

#if defined(_WINDOWS) || defined(_WIN32)
#define strtok_r strtok_s
#endif

char *trim(const char *str) {
    if (str == NULL)
        return NULL;

    if (strlen(str) == 0) {
        return str;
    }

    //when the char is not the white space and not end of file
    const char *head_addr = str;
    const char *tail_addr = str + strlen(str) - 1;

    char *current_addr = head_addr;
    while (*current_addr <= ' ') {
        current_addr++;
        //we already reached the last char, empty
        if (current_addr >= tail_addr) {
            return "";
        }
    }

    const char *valid_begin = current_addr;

    //now we try to trim end
    current_addr = tail_addr;

    while (*current_addr <= ' ') {
        current_addr--;
    }

    //not empty
    const size_t length = current_addr + 1 - valid_begin;
    if (length > 0) {
        *(current_addr + 1) = '\0';
        return valid_begin;
    } else {
        return "";
    }
}

char *split(char *origin_str, const char *delimiter, char **cursor) {
    return strtok_r(origin_str, delimiter, cursor);
}
