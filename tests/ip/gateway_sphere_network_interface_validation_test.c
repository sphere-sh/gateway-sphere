#include <string.h>

#include "../../libs/unity/src/unity.h"
#include "../../src/ip/gateway_sphere_network_interface.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_returns_checksum_mismatch_when_checksum_on_settings_is_incorrect(void) {
    struct gateway_sphere_network_interface_settings_t settings;
    snprintf(settings.interface_name, sizeof(settings.interface_name), "eth0");
    snprintf(settings.ip_address, sizeof(settings.ip_address), "127.0.0.1");
    settings.port = 8080;
    settings.enabled = 1;
    settings.checksum = 0; // Intentionally incorrect checksum

    int result = gateway_sphere_network_interface_validate_settings(&settings);
    TEST_ASSERT_EQUAL_INT(result, GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_CHECKSUM_MISMATCH);
}

static void test_returns_invalid_interface_name_when_name_is_empty(void) {
    struct gateway_sphere_network_interface_settings_t settings;
    snprintf(settings.interface_name, sizeof(settings.interface_name), "");
    snprintf(settings.ip_address, sizeof(settings.ip_address), "127.0.0.1");
    settings.port = 8080;
    settings.enabled = 1;
    settings.checksum = 0; // Intentionally incorrect checksum

    // Calculate checksum
    int32_t checksum = 0;
    for (size_t i = 0; i < sizeof(settings) - sizeof(settings.checksum); i++) {
        checksum += ((uint8_t *) &settings)[i];
    }
    settings.checksum = checksum;

    int result = gateway_sphere_network_interface_validate_settings(&settings);
    TEST_ASSERT_EQUAL_INT(result, GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_INTERFACE_NAME);
}

static void test_returns_invalid_interface_name_when_name_is_null_terminated(void) {
    struct gateway_sphere_network_interface_settings_t settings;
    memset(settings.interface_name, 'A', sizeof(settings.interface_name)); // No null terminator
    snprintf(settings.ip_address, sizeof(settings.ip_address), "127.0.0.1");
    settings.port = 8080;
    settings.enabled = 1;
    settings.checksum = 0; // Intentionally incorrect checksum

    // Calculate checksum
    int32_t checksum = 0;
    for (size_t i = 0; i < sizeof(settings) - sizeof(settings.checksum); i++) {
        checksum += ((uint8_t *) &settings)[i];
    }
    settings.checksum = checksum;

    int result = gateway_sphere_network_interface_validate_settings(&settings);
    TEST_ASSERT_EQUAL_INT(result, GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_INTERFACE_NAME);
}

static void test_returns_invalid_ip_address_when_ip_is_malformed(void) {
    struct gateway_sphere_network_interface_settings_t settings;
    snprintf(settings.interface_name, sizeof(settings.interface_name), "eth0");
    snprintf(settings.ip_address, sizeof(settings.ip_address), "999.999.999.999"); // Invalid IP
    settings.port = 8080;
    settings.enabled = 1;
    settings.checksum = 0; // Intentionally incorrect checksum

    // Calculate checksum
    int32_t checksum = 0;
    for (size_t i = 0; i < sizeof(settings) - sizeof(settings.checksum); i++) {
        checksum += ((uint8_t *) &settings)[i];
    }
    settings.checksum = checksum;

    int result = gateway_sphere_network_interface_validate_settings(&settings);
    TEST_ASSERT_EQUAL_INT(result, GATEWAY_SPHERE_NETWORK_INTERFACE_SETTINGS_VALIDATION_ERROR_INVALID_IP_ADDRESS);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_returns_checksum_mismatch_when_checksum_on_settings_is_incorrect);
    RUN_TEST(test_returns_invalid_interface_name_when_name_is_empty);
    RUN_TEST(test_returns_invalid_interface_name_when_name_is_null_terminated);
    RUN_TEST(test_returns_invalid_ip_address_when_ip_is_malformed);

    return UNITY_END();
}
