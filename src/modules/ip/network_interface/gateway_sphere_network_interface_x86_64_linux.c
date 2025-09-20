#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
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
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
            struct sockaddr_in *netmask = (struct sockaddr_in *)ifa->ifa_netmask;

            if (addr) {
                char ip_buffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(addr->sin_addr), ip_buffer, INET_ADDRSTRLEN);
                snprintf(interfaces->interfaces[index].ip_address_v4, sizeof(interfaces->interfaces[index].ip_address_v4),
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
            struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)ifa->ifa_addr;
            if (addr6) {
                char ip6_buffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &(addr6->sin6_addr), ip6_buffer, INET6_ADDRSTRLEN);
                snprintf(interfaces->interfaces[index].ip_address_v6, sizeof(interfaces->interfaces[index].ip_address_v6),
                        "%s", ip6_buffer);
            }
        }

        index++;
    }

    freeifaddrs(ifaddr); // Frees memory allocated by getifaddrs.

    return GATEWAY_SPHERE_NETWORK_INTERFACES_GET_ALL_OF_HOST_SUCCESS;
}
