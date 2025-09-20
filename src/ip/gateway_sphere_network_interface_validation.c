#include "gateway_sphere_network_interface.h"
#include <stdio.h>
#include <string.h>

static uint32_t gateway_sphere_network_interface_settings_calculate_check_sum(
    struct gateway_sphere_network_interface_settings_t *settings) {
    uint32_t checksum = 0;
    uint8_t *data = (uint8_t *) settings;
    size_t size = sizeof(*settings) - sizeof(settings->checksum);

    for (size_t i = 0; i < size; i++) {
        checksum += data[i];
    }
    return checksum;
}


static int validate_interface_name(const char *name) {
    return (name[0] != '\0' && strlen(name) < 256) ? 0 : -1;
}

/**
  * @brief Validates an IPv4 address in dotted-decimal notation.
  *
  * @param {const char *} ip_address The IPv4 address to validate.
  *
  * @return {0} if valid, negative error code if invalid.
  */
 static int validate_ipv4_address(const char *ip_address) {
     int segments = 0;
     const char *start = ip_address;

     for (const char *p = ip_address; ; p++) {
         if (*p == '.' || *p == '\0') {
             int len = p - start;
             if (len == 0 || len > 3) return -1;

             segments++;
             if (*p == '\0') break;

             start = p + 1;
         } else if (*p < '0' || *p > '9') {
             return -1;
         }
     }

     return (segments == 4) ? 0 : -1;
 }

static int validate_port(int port) {
    return (port >= 0 && port <= 65535) ? 0 : -1;
}

static int validate_enabled_flag(int enabled) {
    return (enabled == 0 || enabled == 1) ? 0 : -1;
}

/**
 * @brief Validates the given network interface settings. This validation is used to ensure correctness and safety
 * of the settings after loading them from the filesystem.
 *
 * @param {struct gateway_sphere_network_interface_settings_t *} settings Pointer to the settings struct to validate.
 *
 * @return {0} if valid, negative error code if invalid.
 */
int gateway_sphere_network_interface_validate_settings(
    struct gateway_sphere_network_interface_settings_t *settings) {

    uint32_t calculated = gateway_sphere_network_interface_settings_calculate_check_sum(settings);
    if (calculated != settings->checksum) {
        fprintf(stderr, "Settings checksum mismatch\n");
        return GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_CHECKSUM_MISMATCH;
    }

    if (validate_interface_name(settings->interface_name) != 0) {
        fprintf(stderr, "Invalid interface name: %s\n", settings->interface_name);
        return GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_INTERFACE_NAME;
    }

    if (validate_enabled_flag(settings->enabled) != 0) {
        fprintf(stderr, "Invalid enabled flag: %d\n", settings->enabled);
        return GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_ENABLED_FLAG;
    }

    if (validate_ipv4_address(settings->ip_address) != 0) {
        fprintf(stderr, "Invalid IP address: %s\n", settings->ip_address);
        return GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_IP_ADDRESS;
    }

    if (validate_port(settings->port) != 0) {
        fprintf(stderr, "Invalid port: %d\n", settings->port);
        return GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_PORT;
    }

    return GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_OK;
}