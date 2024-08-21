cmake_minimum_required(VERSION 3.29)
project(sharing_server C)

set(CMAKE_C_STANDARD 23)

find_package(OpenSSL REQUIRED)

add_executable(Client Client/client.c libs/ssl.c libs/hash.c)
target_link_libraries(Client OpenSSL::SSL OpenSSL::Crypto)

add_executable(Server Server/server.c libs/ssl.c libs/hash.c)
target_link_libraries(Server OpenSSL::SSL OpenSSL::Crypto)