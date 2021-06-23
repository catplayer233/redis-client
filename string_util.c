//
// Created by mahongxu on 6/23/2021.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "string_util.h"

char *trim(const char *str) {
    if (str == NULL)
        return NULL;
    //when the char is not the white space and not end of file
    const char *head_addr = str;
    const char *tail_addr = str + strlen(str);
    while (*head_addr <= ' ')
        head_addr++;
    while (*tail_addr <= ' ')
        tail_addr--;
    //not empty
    const size_t length = tail_addr + 1 - head_addr;
    if (length > 0) {
        char *target = malloc(length + 1);
        strncpy(target, head_addr, length);
        //end of file
        *(target + length + 1) = '\0';
        return target;
    } else
        return NULL;
}
