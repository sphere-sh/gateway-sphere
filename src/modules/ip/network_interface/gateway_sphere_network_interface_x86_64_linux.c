#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include "gateway_sphere_network_interface.h"
#include <arpa/inet.h>
#include <netinet/in.h>

int gateway_sphere_network_interfaces_get_all_of_host(gateway_sphere_network_interfaces *interfaces) {
    struct ifaddrs *ifaddr;
    if (getifaddrs(&ifaddr) == -1) {
        return GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_FAILURE;
    }

    // Count the length of the ifaddr
    //
    struct ifaddrs *ifa;
    int count = 0;
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        count++;
    }

    // Allocate memory for the interfaces
    //
    interfaces->interfaces = malloc(sizeof(struct gateway_sphere_network_interface_t) * count);
    if (interfaces->interfaces == NULL) {
        freeifaddrs(ifaddr);
        return GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_FAILURE;
    }
    interfaces->interface_count = count;

    // Populate the interfaces
    //
    int index = 0;
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        // Skip entries with no address
        if (ifa->ifa_addr == NULL)
            continue;

        snprintf(interfaces->interfaces[index].name, sizeof(interfaces->interfaces[index].name), "%s", ifa->ifa_name);
        interfaces->interfaces[index].is_up = (ifa->ifa_flags & IFF_UP) ? 1 : 0;

        // Initialize with default values
        snprintf(interfaces->interfaces[index].mac_address, sizeof(interfaces->interfaces[index].mac_address),
                 "%s", "00:00:00:00:00:00");
        snprintf(interfaces->interfaces[index].ip_address_v4, sizeof(interfaces->interfaces[index].ip_address_v4),
                 "%s", "0.0.0.0");
        snprintf(interfaces->interfaces[index].ip_address_v6, sizeof(interfaces->interfaces[index].ip_address_v6),
                 "%s", "::");
        snprintf(interfaces->interfaces[index].subnet_mask, sizeof(interfaces->interfaces[index].subnet_mask),
                 "%s", "255.255.255.0");
        snprintf(interfaces->interfaces[index].gateway, sizeof(interfaces->interfaces[index].gateway),
                 "%s", "0.0.0.0");
        snprintf(interfaces->interfaces[index].dns_server, sizeof(interfaces->interfaces[index].dns_server),
                 "%s", "0.0.0.0");

        // Handle ipv4 addresses
        //
        if (ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *) ifa->ifa_addr;
            struct sockaddr_in *netmask = (struct sockaddr_in *) ifa->ifa_netmask;

            if (addr) {
                char ip_buffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(addr->sin_addr), ip_buffer, INET_ADDRSTRLEN);
                snprintf(interfaces->interfaces[index].ip_address_v4,
                         sizeof(interfaces->interfaces[index].ip_address_v4),
                         "%s", ip_buffer);
            }

            if (netmask) {
                char mask_buffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(netmask->sin_addr), mask_buffer, INET_ADDRSTRLEN);
                snprintf(interfaces->interfaces[index].subnet_mask, sizeof(interfaces->interfaces[index].subnet_mask),
                         "%s", mask_buffer);
            }
        }

        // Handle ipv6 addresses
        //
        if (ifa->ifa_addr->sa_family == AF_INET6) {
            struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) ifa->ifa_addr;
            if (addr6) {
                char ip6_buffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &(addr6->sin6_addr), ip6_buffer, INET6_ADDRSTRLEN);
                snprintf(interfaces->interfaces[index].ip_address_v6,
                         sizeof(interfaces->interfaces[index].ip_address_v6),
                         "%s", ip6_buffer);
            }
        }

        index++;
    }

    freeifaddrs(ifaddr); // Frees memory allocated by getifaddrs.

    return GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_SUCCESS;
}

struct gateway_sphere_network_interface_creation_result_t *gateway_sphere_network_interface_create_virtual(
    gateway_sphere_network_interfaces * interfaces, const char *name, const char *mac_address, const char *ip_address_v4,
    const char *ip_address_v6, const char *subnet_mask, const char *gateway, const char *dns_server, int is_up) {

    struct gateway_sphere_network_interface_creation_result_t * result = malloc(sizeof(struct gateway_sphere_network_interface_creation_result_t));
    if (result == NULL) {
        return NULL;
    }

    // Ensure uniqueness of parameters.
    //
    for (int i = 0; i < interfaces->interface_count; i++) {

        // Check for unique name
        //
        if (strcmp(interfaces->interfaces[i].name, name) == 0) {
            result->code = GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_NAME_ALREADY_EXISTS;
            return result;
        }

        // Check for unique MAC address
        //
        if (strcmp(interfaces->interfaces[i].mac_address, mac_address) == 0) {
            result->code = GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_MAC_ADDRESS_ALREADY_EXISTS;
            return result;
        }

        // Check for unique IPv4 address
        //
        if (strcmp(interfaces->interfaces[i].ip_address_v4, ip_address_v4) == 0) {
            result->code = GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_IP_ADDRESS_V4_ALREADY_EXISTS;
            return result;
        }
    }


    // Create the structure to hold the network interface information
    //
    struct gateway_sphere_network_interface_t *interface = malloc(sizeof(struct gateway_sphere_network_interface_t));
    if (interface == NULL) {
        return NULL;
    }

    snprintf(interface->name, sizeof(interface->name), "%s", name);
    snprintf(interface->mac_address, sizeof(interface->mac_address), "%s", mac_address);
    snprintf(interface->ip_address_v4, sizeof(interface->ip_address_v4), "%s", ip_address_v4);
    snprintf(interface->ip_address_v6, sizeof(interface->ip_address_v6), "%s", ip_address_v6);
    snprintf(interface->subnet_mask, sizeof(interface->subnet_mask), "%s", subnet_mask);
    snprintf(interface->gateway, sizeof(interface->gateway), "%s", gateway);
    snprintf(interface->dns_server, sizeof(interface->dns_server), "%s", dns_server);
    interface->is_up = is_up;

    // Create a virtual network interface in the system (Linux specific).
    //
    char command[256];
    snprintf(command, sizeof(command),
             "sudo ip link add %s type dummy && "
             "sudo ip link set %s address %s && "
             "sudo ip addr add %s dev %s && "
             "sudo ip link set %s %s",
             name, name, mac_address, ip_address_v4, name, name, is_up ? "up" : "down");
    system(command);


    result->code = GATEWAY_SPHERE_NETWORK_INTERFACES_CREATE_VIRTUAL_RESULT_CODE_SUCCESS;
    result->interface = interface;

    return result;
}
