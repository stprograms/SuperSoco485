#include <unity.h>

#include "BatteryStatus.h"

using namespace stprograms::SuperSoco485;

static uint8_t valid_battery_telegram[] = {0xB6, 0x6B, 0xAA, 0x5A, 0x0A, 0x4D, 0x48, 0x17, 0x00, 0x00, 0x23, 0x00, 0x0B, 0x00, 0x00, 0x30, 0x0D};

void test_is_battery_status()
{
    BaseTelegram baseTel(valid_battery_telegram, sizeof(valid_battery_telegram));
    TEST_ASSERT_TRUE(BatteryStatus::isBatteryStatusTelegram(baseTel));
}

void test_is_not_battery_status()
{
    uint8_t invalid_battery_telegram[] = {0xB6, 0x6B, 0xAA, 0xDA, 0x0A, 0x4D, 0x48, 0x17, 0x00, 0x00, 0x23, 0x00, 0x0B, 0x00, 0x00, 0x30, 0x0D};
    BaseTelegram baseTel(invalid_battery_telegram, sizeof(invalid_battery_telegram));
    TEST_ASSERT_FALSE(BatteryStatus::isBatteryStatusTelegram(baseTel));
}

void test_parse_battery_status()
{
    BaseTelegram baseTel(valid_battery_telegram, sizeof(valid_battery_telegram));
    BatteryStatus batteryStatus(baseTel);

    TEST_ASSERT_TRUE(batteryStatus.isValid());
    TEST_ASSERT_EQUAL(0x4D, batteryStatus.getVoltage());
    TEST_ASSERT_EQUAL(0x48, batteryStatus.getSoC());
    TEST_ASSERT_EQUAL(0x17, batteryStatus.getTemperature());
    TEST_ASSERT_EQUAL(0x00, batteryStatus.getChargeCurrent());
    TEST_ASSERT_EQUAL(0x0023, batteryStatus.getLoadCycles());
    TEST_ASSERT_EQUAL(0x000B, batteryStatus.getDischargeCycles());
    TEST_ASSERT_EQUAL(0x00, batteryStatus.getErrorCode());
    TEST_ASSERT_EQUAL(BatteryActivity::UNKNOWN_ACTIVITY, batteryStatus.getActivity());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_is_battery_status);
    RUN_TEST(test_is_not_battery_status);
    RUN_TEST(test_parse_battery_status);
    return UNITY_END();
}
