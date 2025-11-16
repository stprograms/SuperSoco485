#include <unity.h>

#include "MCUResponse.h"
#include "test_mcu_response.h"

using namespace stprograms::SuperSoco485;

static uint8_t valid_mcu_response[] = {0xB6, 0x6B, 0xAA, 0xDA, 0x0A, 0x02, 0x00, 0x04, 0x00, 0x00, 0x13, 0x00, 0x00, 0x02, 0x01, 0x1C, 0x0D};

static void test_is_mcu_response()
{
    BaseTelegram baseTel(valid_mcu_response, sizeof(valid_mcu_response));
    TEST_ASSERT_TRUE(MCUResponse::isMCUResponseTelegram(baseTel));
}

static void test_is_not_mcu_response()
{
    uint8_t invalid_response[] = {0xB6, 0x6B, 0xAA, 0x5A, 0x0A, 0x4D, 0x48, 0x17, 0x00, 0x00, 0x23, 0x00, 0x0B, 0x00, 0x00, 0x30, 0x0D};
    BaseTelegram baseTel(invalid_response, sizeof(invalid_response));
    TEST_ASSERT_FALSE(MCUResponse::isMCUResponseTelegram(baseTel));
}

static void test_parse_mcu_response()
{
    BaseTelegram base = BaseTelegram(valid_mcu_response, sizeof(valid_mcu_response));
    MCUResponse response = MCUResponse(base);

    TEST_ASSERT_EQUAL_UINT8(2, response.getGear());
    TEST_ASSERT_EQUAL_INT16(4, response.getCurrent());
    TEST_ASSERT_EQUAL_UINT16(0, response.getSpeed());
    TEST_ASSERT_EQUAL_INT8(19, response.getTemperature());

#if (SUPER_SOCO_PARKING_INVERT == 0)
    TEST_ASSERT_TRUE(response.isParking());
#else
    TEST_ASSERT_FALSE(response.isParking());
#endif
}

void run_mcu_response_tests(void)
{
    RUN_TEST(test_is_mcu_response);
    RUN_TEST(test_is_not_mcu_response);
    RUN_TEST(test_parse_mcu_response);
}
