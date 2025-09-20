#include "gateway_sphere_network_interface.h"
#include "gateway_sphere_network_interface_validation.c"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ifaddrs.h>

static struct gateway_sphere_network_interface_module_t * gateway_sphere_network_interface_module;

static struct gateway_sphere_network_interface_settings_t * gateway_sphere_network_interface_settings;
static struct ifaddrs * gateway_sphere_network_interface_ifaddr;

static int gateway_sphere_network_interface_destroy() {
    return 0;
}

static struct gateway_sphere_network_interface_settings_t *gateway_sphere_network_interface_load_default_settings() {
    struct gateway_sphere_network_interface_settings_t *defaultSettings = malloc(
        sizeof(struct gateway_sphere_network_interface_settings_t));
    if (defaultSettings == NULL) {
        perror("Failed to allocate memory for default settings");
        return NULL;
    }

    // Set default values
    //
    defaultSettings->enabled = 1;
    defaultSettings->port = 8080;
    snprintf(defaultSettings->interface_name, sizeof(defaultSettings->interface_name), "test0");
    snprintf(defaultSettings->ip_address, sizeof(defaultSettings->ip_address), "192.168.0.0");

    uint32_t checksum = gateway_sphere_network_interface_settings_calculate_check_sum(defaultSettings);
    defaultSettings->checksum = checksum;


    return defaultSettings;
}

static int gateway_sphere_network_interface_persist_default_settings(
    struct gateway_sphere_network_interface_settings_t *settings) {
    // Convert struct into binary
    //
    FILE *file = fopen(GATEWAY_SPHERE_NETWORK_INTERFACE_CONFIG_FILE, "wb");
    if (file == NULL) {
        perror("Failed to open configuration file for writing");
        return -1;
    }

    // Write struct to file
    //
    size_t written = fwrite(settings, sizeof(struct gateway_sphere_network_interface_settings_t), 1, file);
    if (written != 1) {
        perror("Failed to write configuration to file");
        fclose(file);
        return -1;
    }

    fclose(file);


    return 0;
}


static struct gateway_sphere_network_interface_settings_t *gateway_sphere_network_interface_load_settings_on_disk() {
    struct gateway_sphere_network_interface_settings_t *diskSettings = malloc(
        sizeof(struct gateway_sphere_network_interface_settings_t));
    if (diskSettings == NULL) {
        perror("Failed to allocate memory for settings");
        return NULL;
    }

    // Open configuration file
    //
    FILE *file = fopen(GATEWAY_SPHERE_NETWORK_INTERFACE_CONFIG_FILE, "rb");
    if (file == NULL) {
        perror("Failed to open configuration file for reading");
        return NULL;
    }

    // Read struct from file
    //
    size_t read = fread(diskSettings, sizeof(struct gateway_sphere_network_interface_settings_t), 1, file);
    if (read != 1) {
        perror("Failed to read configuration from file");
        fclose(file);
        return NULL;
    }

    // Close file
    //
    fclose(file);

    // Before returning, validate the loaded settings to ensure
    // safety and correctness.
    //
    int validationResult = gateway_sphere_network_interface_validate_settings(diskSettings);
    if (validationResult != 0) {
        fprintf(stderr, "Loaded settings are invalid: %d\n", validationResult);
        free(diskSettings);
        return NULL;
    }


    // Return loaded settings
    //
    return diskSettings;
}

static int gateway_sphere_network_interface_load_settings() {
    // First, check if the configuration file exists.
    //
    struct stat buffer;
    int configurationFileExists = stat(GATEWAY_SPHERE_NETWORK_INTERFACE_CONFIG_FILE, &buffer) == 0;

    // When it does exist, load the settings from disk.
    //
    struct gateway_sphere_network_interface_settings_t *settings;

    if (configurationFileExists) {
        settings = gateway_sphere_network_interface_load_settings_on_disk();
    } else {
        // When it does not exist, we initialize the default
        // settings and save them to disk.
        //
        settings = gateway_sphere_network_interface_load_default_settings();

        int persistanceResult = gateway_sphere_network_interface_persist_default_settings(settings);
        if (persistanceResult != 0) {
            free(settings);
            return persistanceResult;
        }
    }

    // Put the loaded settings into the global variable.
    //
    gateway_sphere_network_interface_settings = settings;

    return 0;
}

static void gateway_sphere_network_interface_load_host_interfaces() {
    if (getifaddrs(&gateway_sphere_network_interface_ifaddr) == -1) {
        gateway_sphere_network_interface_module->state = GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_ERROR_LOAD_HOST_INTERFACES;
        return;
    }
}

int gateway_sphere_network_interface_init() {
    // First, allocate the module structure.
    //
    gateway_sphere_network_interface_module = malloc(sizeof(struct gateway_sphere_network_interface_module_t));
    if (gateway_sphere_network_interface_module == NULL) {
        perror("Failed to allocate memory for network interface module");
        return -1;
    }
    gateway_sphere_network_interface_module->state = GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_INITIALIZING;

    // Then, load the network interface settings from disk.
    //
    int result = gateway_sphere_network_interface_load_settings();
    if (result != 0) {
        gateway_sphere_network_interface_module->state = GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_ERROR_LOAD_SETTINGS;
        return result;
    }

    // Then, load the network interfaces on the host system.
    //
    gateway_sphere_network_interface_load_host_interfaces();
    if (gateway_sphere_network_interface_module->state == GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_ERROR_LOAD_HOST_INTERFACES) {
        return GATEWAY_SPHERE_NETWORK_INTERFACE_MODULE_STATE_ERROR_LOAD_HOST_INTERFACES;
    }

    // Then, determine the state of network interfaces
    // on the host system.
    //

    // todo: implement this function.

    // Then, act based on the loaded settings and the
    // current state of the network interfaces.
    //

    // todo: implement this function.


    return 0;
}
