#include <stdio.h>
#include <stdlib.h>

#include "modules/ip/network_interface/gateway_sphere_network_interface.h"

int main(void) {

    gateway_sphere_network_interfaces * interfaces = malloc(sizeof(gateway_sphere_network_interfaces));
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

    return 0;
}