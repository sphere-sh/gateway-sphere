#include <stdio.h>
#include <stdlib.h>

#include "modules/ip/network_interface/gateway_sphere_network_interface.h"

int main(void) {
    gateway_sphere_network_interfaces *interfaces = malloc(sizeof(gateway_sphere_network_interfaces));
    if (interfaces == NULL) {
        fprintf(stderr, "Failed to allocate memory for network interfaces structure\n");
        return -1;
    }

    int result = gateway_sphere_network_interfaces_get_all_of_host(interfaces);
    if (result == GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_FAILURE) {
        fprintf(stderr, "Failed to get network interfaces of the host\n");
        free(interfaces);
        return -1;
    }

    // Create a virtual network interface for testing purposes
    //
    struct gateway_sphere_network_interface_creation_result_t * result2 = gateway_sphere_network_interface_create_virtual(
        interfaces,
        "veth0",
        "AA:BB:CC:DD:EE:FF",
        "192.168.200.0",
        "::1",
        "",
        "",
        "",
        1
    );

    if (result2->code != GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_SUCCESS) {
        fprintf(stderr, "Failed to create virtual network interface, code: %d\n", result2->code);
        free(interfaces->interfaces);
        free(interfaces);
        free(result2);
        return -1;
    }

    return 0;
}
