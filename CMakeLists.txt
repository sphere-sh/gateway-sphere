cmake_minimum_required(VERSION 4.0)
project(gateway_sphere C)

set(CMAKE_C_STANDARD 23)

add_subdirectory("libs/unity")

add_subdirectory("tests/linux/x86_64")

add_executable(
        gateway_sphere_x86_64_linux
        src/main.c
        src/modules/ip/network_interface/gateway_sphere_network_interface.h
        src/modules/ip/network_interface/gateway_sphere_network_interface_x86_64_linux.c
)


#target_link_libraries(
#        gateway_sphere_network_interface_validation_test
#        PRIVATE
#        unity::framework
#)

