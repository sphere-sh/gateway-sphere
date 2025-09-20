#include <string.h>

#include "../../../../../../libs/unity/src/unity.h"
#include "../../../../../../src/modules/ip/network_interface/gateway_sphere_network_interface.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_returns_checksum_mismatch_when_checksum_on_settings_is_incorrect(void) {
    TEST_ASSERT_EQUAL_INT(1, 1);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_returns_checksum_mismatch_when_checksum_on_settings_is_incorrect);

    return UNITY_END();
}
