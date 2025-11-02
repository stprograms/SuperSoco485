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

    RUN_TEST(test_telegram_parser_initial_state);
    RUN_TEST(test_telegram_parser_stored_data);
    RUN_TEST(test_telegram_parser_flush);
    RUN_TEST(test_telegram_parser_incomplete_no_callback);
    RUN_TEST(test_telegram_parser_incomplete_and_new_no_callback);
    RUN_TEST(test_telegram_parser_complete_telegram_callback);
    RUN_TEST(test_telegram_parser_incomplete_dropped);
    RUN_TEST(test_telegram_parser_states);

    return UNITY_END();
}
