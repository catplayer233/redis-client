//
// Created by mahongxu on 6/23/2021.
//

#ifndef REDIS_CLIENT_UTIL_COMMANDS_H
#define REDIS_CLIENT_UTIL_COMMANDS_H

#include "../protocol/redis_data_type.h"

#define auth(pwd, command_buf) make_command((char *[]){"AUTH",(pwd)},2,(command_buf))
#define auth_with_user(user_name, pwd, command_buf) make_command((char *[]){(user_name),"AUTH",(pwd)},2,(command_buf))
#endif //REDIS_CLIENT_UTIL_COMMANDS_H
