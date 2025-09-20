#ifndef GATEWAY_SPHERE_GATEWAY_SPHERE_NETWORK_INTERFACE_H
#define GATEWAY_SPHERE_GATEWAY_SPHERE_NETWORK_INTERFACE_H

#define GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_FAILURE (-1)
#define GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_SUCCESS 0

#define GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_SUCCESS 0
#define GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_NAME_ALREADY_EXISTS 1
#define GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_MAC_ADDRESS_ALREADY_EXISTS 2
#define GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_IP_ADDRESS_V4_ALREADY_EXISTS 3
#define GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_IP_ADDRESS_V6_ALREADY_EXISTS 4

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

struct gateway_sphere_network_interface_creation_result_t {
    /**
     * Result code of the creation operation. 0 indicates success, non-zero
     * indicates failure.
     */
    int code;
    struct gateway_sphere_network_interface_t *interface;
};

/**
 * @brief Get all network interfaces of the host system.
 *
 * @param {gateway_sphere_network_interfaces *} interfaces - Pointer to a structure to hold the network interfaces.
 *
 * @return {int} - Returns GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_SUCCESS on success, GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_FAILURE on failure.
 */
int gateway_sphere_network_interfaces_get_all_of_host(gateway_sphere_network_interfaces *interfaces);

/**
 * @brief Create a virtual network interface with specified parameters.
 *
 * @param {gateway_sphere_network_interfaces *} interfaces - Pointer to the structure holding existing network interfaces.
 * @param {const char *} name - Name of the network interface.
 * @param {const char *} mac_address - MAC address of the network interface.
 * @param {const char *} ip_address_v4 - IPv4 address of the network interface.
 * @param {const char *} ip_address_v6 - IPv6 address of the network interface.
 * @param {const char *} subnet_mask - Subnet mask of the network interface.
 * @param {const char *} gateway - Gateway of the network interface.
 * @param {const char *} dns_server - DNS server of the network interface.
 * @param {int} is_up - Status of the network interface (1 for up, 0 for down).
 *
 * @return {struct gateway_sphere_network_interface_creation_result_t *}
 */
struct gateway_sphere_network_interface_creation_result_t *gateway_sphere_network_interface_create_virtual(
    gateway_sphere_network_interfaces * interfaces, const char *name, const char *mac_address, const char *ip_address_v4,
    const char *ip_address_v6, const char *subnet_mask, const char *gateway, const char *dns_server, int is_up);

#endif //GATEWAY_SPHERE_GATEWAY_SPHERE_NETWORK_INTERFACE_H
