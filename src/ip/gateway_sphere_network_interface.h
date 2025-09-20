#ifndef GATEWAY_SPHERE_GATEWAY_SPHERE_NETWORK_INTERFACE_H
#define GATEWAY_SPHERE_GATEWAY_SPHERE_NETWORK_INTERFACE_H

#include <stdint.h>

#define GATEWAY_SPHERE_NETWORK_INTERFACE_CONFIG_FILE "/etc/sphere/gateway/network_interfaces/conf"

/**
 * Module state definitions.
 */
#define GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_ERROR_LOAD_HOST_INTERFACES (-3)
#define GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_ERROR_LOAD_SETTINGS (-2)
#define GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_ERROR (-1)
#define GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_UNINITIALIZED 0
#define GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_INITIALIZING 1
#define GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_INITIALIZED 2

/**
 * Settings validation error codes.
 */
#define GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_OK 0
#define GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_CHECKSUM_MISMATCH -1
#define GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_IP_ADDRESS -2
#define GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_PORT -3
#define GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_ENABLED_FLAG -4
#define GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_INTERFACE_NAME -5

static struct gateway_sphere_network_interface_module_t {
    int state;
};



/**
 * @brief Representation of a network interface in the Gateway Sphere system. A network
 * interface is backed by a physical or virtual network device on the host system.
 */
static struct gateway_sphere_network_interface_t {
    /**
     * @brief The name of the network interface (e.g., "eth0", "wlan0"). The name
     * must match an existing network interface on the host system.
     */
    char name[15];
};

static struct gateway_sphere_network_interface_settings_t {
    char interface_name[256];
    char ip_address[16];
    int port;
    int enabled;
    uint32_t checksum; // Add integrity check
};

/**
 * @brief Collection of all network interfaces in the Gateway Sphere system.
 */
static struct {
    /**
     * @brief Array of network interfaces.
     */
    struct gateway_sphere_network_interface_t *interfaces;
    /**
     * @brief Number of network interfaces in the array.
     */
    int count;
} gateway_sphere_network_interfaces_t;

/**
 * @brief  Cleans up and frees resources associated with the network interface module.
 * @return {0} on success, negative error code on failure.
 */
static int gateway_sphere_network_interface_destroy();

/**
 * @brief Load network interface settings from configuration file
 * on disk.
 *
 * @return {0} on success, negative error code on failure.
 */
static int gateway_sphere_network_interface_load_settings();

/**
 * @brief Initializes the network interface module.
 * @return {0} on success, negative error code on failure.
 */
int gateway_sphere_network_interface_init();

#endif //GATEWAY_SPHERE_GATEWAY_SPHERE_NETWORK_INTERFACE_H
