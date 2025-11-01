#include <unity.h>
#include "test_super_soco.h"

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_super_soco_init_no_param);
    RUN_TEST(test_super_soco_init_with_param);
    RUN_TEST(test_super_soco_parse_0_chunk);
    RUN_TEST(test_super_soco_data_updated);
    RUN_TEST(test_super_soco_battery_data);
    RUN_TEST(test_super_soco_ecu_data);
    return UNITY_END();
}
