#include <stdio.h>

#include "ip/gateway_sphere_network_interface.h"

int main(void) {

    // Start by initializing the network interface module.
    //
    int result = gateway_sphere_network_interface_init();
    if (result != 0) {
        fprintf(stderr, "Failed to initialize network interface module: %d\n", result);
        return result;
    }



    // Stopping the network interface module.
    //


    return 0;
}