

add_executable(
        gateway_sphere_network_interface_x86_64_linux_test
        modules/ip/network_interface/gateway_sphere_network_interface_x86_64_linux_test.c
        ../../../src/modules/ip/network_interface/gateway_sphere_network_interface.h
        ../../../src/modules/ip/network_interface/gateway_sphere_network_interface_x86_64_linux.c
)

target_link_libraries(
        gateway_sphere_network_interface_x86_64_linux_test
        PRIVATE
        unity::framework
)



add_test(
        NAME "Network Interface x86_64 Linux Test"
        COMMAND gateway_sphere_network_interface_x86_64_linux_test
)