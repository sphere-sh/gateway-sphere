#ifndef GATEWAY_SPHERE_GATEWAY_SPHERE_NETWORK_INTERFACE_H
#define GATEWAY_SPHERE_GATEWAY_SPHERE_NETWORK_INTERFACE_H

#define GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_FAILURE (-1)
#define GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_SUCCESS 0

struct gateway_sphere_network_interface_t {
    char name[32];
    char mac_address[18];
    char ip_address_v4[16];
    char ip_address_v6[40];
    char subnet_mask[16];
    char gateway[16];
    char dns_server[16];
    int is_up;
};

typedef struct gateway_sphere_network_interfaces_t {
    struct gateway_sphere_network_interface_t *interfaces;
    int interface_count;
} gateway_sphere_network_interfaces;

/**
 * @brief Get all network interfaces of the host system.
 *
 * @param {gateway_sphere_network_interfaces *} interfaces - Pointer to a structure to hold the network interfaces.
 * @return {int} - Returns GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_SUCCESS on success, GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_FAILURE on failure.
 */
int gateway_sphere_network_interfaces_get_all_of_host(gateway_sphere_network_interfaces *interfaces);

#endif //GATEWAY_SPHERE_GATEWAY_SPHERE_NETWORK_INTERFACE_H
