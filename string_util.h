//
// Created by mahongxu on 6/23/2021.
//

#ifndef REDIS_CLIENT_STRING_UTIL_H
#define REDIS_CLIENT_STRING_UTIL_H

#endif //REDIS_CLIENT_STRING_UTIL_H

char *trim(const char *str);

char *split(char *origin_str, const char *delimiter, char **cursor);

int is_empty(char *target_str);
