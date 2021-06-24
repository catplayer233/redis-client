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

//this macro will produce a string for the target data type
#define type_prefix(_data_type) (char []){_data_type,'\0'}

#endif //REDIS_CLIENT_REDIS_DATA_TYPE_H
