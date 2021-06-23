//
// Created by mahongxu on 6/21/2021.
//

#ifndef REDIS_CLIENT_REDIS_DATA_TYPE_H
#define REDIS_CLIENT_REDIS_DATA_TYPE_H

#define SIMPLE_STRINGS '+'
#define ERRORS '-'
#define INTEGERS ':'
#define BULK_STRINGS '$'
#define ARRAYS '*'
#define TERMINATED_SYMBOL "\r\n"
#define COMMAND_SIZE 1024

typedef unsigned int _arg_num;

/*
 * provide an useful function for producing a redis command request
 * as the RESP: https://redis.io/topics/protocol
 * a redis command request is a bulk string array
 *
 * like: *(arguments number)\r\n$(argument bytes length)\r\n(content)\r\n...
 *
 * note: we make you to provide the buf for keeping the target command_buf be freed when you have done with this,
 * we do not alloc extra space when you invoke this function
 */
char *make_command(char **command_args, _arg_num arg_num, char *command_buf);

#endif //REDIS_CLIENT_REDIS_DATA_TYPE_H
