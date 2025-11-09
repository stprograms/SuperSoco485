
#include <unity.h>
#include "super_soco/test_super_soco.h"
#include "telegram_parser/test_telegram_parser.h"

#include "telegrams/battery_status/test_battery_status.h"

int main(void)
{
    UNITY_BEGIN();
    run_super_soco_tests();
    run_telegram_parser_tests();
    run_telegram_battery_status_tests();
    return UNITY_END();
}
