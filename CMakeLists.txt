# CMakeList.txt : CMake project for redis-client, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project("redis-client")

# Add source to this project's executable.
add_executable(redis-client "cli.c" "client_config.h" "client_config.c" "client_socket.c" "client_socket.h" "protocol/redis_data_type.h" command/commands_utils.c command/commands_utils.h string_util.c string_util.h)

# TODO: Add tests and install targets if needed.
