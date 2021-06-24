//
// Created by mahongxu on 6/23/2021.
//

#ifndef REDIS_CLIENT_STRING_UTIL_H
#define REDIS_CLIENT_STRING_UTIL_H

#endif //REDIS_CLIENT_STRING_UTIL_H

typedef enum boolean {
    FALSE,
    TRUE
} boolean;

char *trim(char *str);

/*
 * 0 means false, non-zero means true
 */
boolean check_num(char *str);
