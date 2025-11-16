#include <unity.h>
#include <stdio.h>
#include "SuperSoco485.h"
#include "test_super_soco.h"

using namespace stprograms::SuperSoco485;

#pragma region Mocking
class MockSuperSoco485 : public SuperSoco485
{
public:
    MockSuperSoco485() : SuperSoco485() {}

    DataChangedHandler getVehicleDataUpdatedHandler() const
    {
        return vehicleDataUpdatedHandler;
    }

    void *getUserData() const
    {
        return _user_data;
    }
};
#pragma endregion

#pragma region utility functions
static uint8_t cntHandlerCalls = 0;
static void *lastUserData = nullptr;
static SuperSoco485 *lastSender = nullptr;

static void dataUpdatedHandler(void *user_data, SuperSoco485 *sender)
{
    cntHandlerCalls++;
    lastUserData = user_data;
    lastSender = sender;
}

static uint8_t batteryData[] = {
    0xB6, 0x6B, 0xAA, 0x5A, 0x0A,
    65, 27, 18, 0, 0, 99, 0, 88, 0, 0,
    0x79, 0x0D};

static uint8_t ecuData[] = {
    0xB6, 0x6B, 0xAA, 0xDA, 0x0A,
    3, 0, 0, 0, 0, 24, 0, 0, 2, 0,
    0x13, 0x0D};
#pragma endregion

#pragma region test cases

/**
 * @brief Test initialization without parameters
 */
void test_super_soco_init_no_param(void)
{
    MockSuperSoco485 ss;
    ss.begin();

    TEST_ASSERT_NULL(ss.getVehicleDataUpdatedHandler());
    TEST_ASSERT_NULL(ss.getUserData());
}

/**
 * @brief Test initialization with parameters
 */
void test_super_soco_init_with_param(void)
{
    MockSuperSoco485 ss;
    int userData = 42;

    ss.begin(dataUpdatedHandler, &userData);

    TEST_ASSERT_EQUAL_PTR(dataUpdatedHandler, ss.getVehicleDataUpdatedHandler());
    TEST_ASSERT_EQUAL_PTR(&userData, ss.getUserData());
}

/**
 * @brief Test that parsing a 0-filled chunk is not parsed and no callback is
 * called.
 */
void test_super_soco_parse_0_chunk(void)
{
    MockSuperSoco485 ss;
    uint8_t rawData[24] = {
        0,
    };

    // reset tracking variables
    cntHandlerCalls = 0;
    lastUserData = nullptr;
    lastSender = nullptr;

    // initialize with callback
    ss.begin(dataUpdatedHandler);

    // parse the whole chunk at once
    ss.parseChunk(rawData, sizeof(rawData));

    TEST_ASSERT_EQUAL(0, cntHandlerCalls);

    // parse the chunk byte by byte
    for (uint8_t i = 0; i < sizeof(rawData); i++)
    {
        ss.parseChunk(&rawData[i], 1);
    }

    TEST_ASSERT_EQUAL(0, cntHandlerCalls);
}

/**
 * @brief Test that parsing battery data telegrams calls the callback once.
 */
void test_super_soco_data_updated(void)
{
    MockSuperSoco485 ss;

    // reset tracking variables
    cntHandlerCalls = 0;
    lastSender = nullptr;

    ss.begin(dataUpdatedHandler);

    // parse the telegram in chunks and write first 2 bytes again to trigger
    // processing
    ss.parseChunk(batteryData, sizeof(batteryData));
    ss.parseChunk(batteryData, 2);

    // Test that handler has been called once
    TEST_ASSERT_EQUAL(1, cntHandlerCalls);
    TEST_ASSERT_EQUAL_PTR(&ss, lastSender);

    // parse the same data again (first 2 bytes where sent previously)
    ss.parseChunk(batteryData + 2, sizeof(batteryData) - 2);
    ss.parseChunk(batteryData, 2);

    // Test that handler has not been called since that
    TEST_ASSERT_EQUAL(1, cntHandlerCalls);

    // flush data to reset parser
    ss.flush();

    // write ECU data
    ss.parseChunk(ecuData, sizeof(ecuData));
    ss.parseChunk(batteryData, 2);

    // Test that handler has been called a second time
    TEST_ASSERT_EQUAL(2, cntHandlerCalls);
}

/**
 * @brief Test that parsing battery data telegrams updated the status in the
 * SuperSoco485 instance.
 */
void test_super_soco_battery_data(void)
{
    MockSuperSoco485 ss;

    // reset tracking variables
    cntHandlerCalls = 0;
    lastSender = nullptr;

    ss.begin(dataUpdatedHandler);

    ss.parseChunk(batteryData, sizeof(batteryData));
    ss.parseChunk(batteryData, 2); // trigger processing

    // Test that handler has been called once
    TEST_ASSERT_EQUAL(1, cntHandlerCalls);
    TEST_ASSERT_EQUAL_PTR(&ss, lastSender);

    // Check some data
    TEST_ASSERT_EQUAL(65, ss._status.batVoltage);
    TEST_ASSERT_EQUAL(27, ss._status.Soc);
    TEST_ASSERT_EQUAL(18, ss._status.batTemp);
    TEST_ASSERT_EQUAL(0, ss._status.chargeCurrent);
    TEST_ASSERT_EQUAL(99, ss._status.chargeCycle);
}

/**
 * @brief Test that parsing battery data telegrams updated the status in the
 * SuperSoco485 instance.
 */
void test_super_soco_ecu_data(void)
{
    MockSuperSoco485 ss;

    // reset tracking variables
    cntHandlerCalls = 0;
    lastSender = nullptr;

    ss.begin(dataUpdatedHandler);

    ss.parseChunk(ecuData, sizeof(ecuData));
    ss.parseChunk(ecuData, 2); // trigger processing

    // Test that handler has been called once
    TEST_ASSERT_EQUAL(1, cntHandlerCalls);
    TEST_ASSERT_EQUAL_PTR(&ss, lastSender);

    // Check some data
    TEST_ASSERT_EQUAL(3, ss._status.driveMode);
    TEST_ASSERT_EQUAL(24, ss._status.ecuTemp);
    TEST_ASSERT_TRUE(ss._status.parking);
}

#pragma endregion

/**
 * @brief Execute the super soco tests
 */
void run_super_soco_tests()
{
    RUN_TEST(test_super_soco_init_no_param);
    RUN_TEST(test_super_soco_init_with_param);
    RUN_TEST(test_super_soco_parse_0_chunk);
    RUN_TEST(test_super_soco_data_updated);
    RUN_TEST(test_super_soco_battery_data);
    RUN_TEST(test_super_soco_ecu_data);
}
