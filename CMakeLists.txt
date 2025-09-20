cmake_minimum_required(VERSION 4.0)
project(gateway_sphere C)

set(CMAKE_C_STANDARD 23)

add_executable(
        gateway_sphere
        src/main.c
        src/ip/gateway_sphere_network_interface.h
        src/ip/gateway_sphere_network_interface.c
)
