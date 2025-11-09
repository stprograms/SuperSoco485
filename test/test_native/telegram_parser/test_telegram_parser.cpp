/**
 * @file test/test_native/test_telegram_parser.cpp
 * @author Andreas Kurzmann
 * @brief Unit tests for TelegramParser
 */

#include <unity.h>
#include <TelegramParser.h>
#include <BatteryStatus.h>

#include "test_telegram_parser.h"

using namespace stprograms::SuperSoco485;

#pragma region Helper classes and functions
class MockParser : public TelegramParser
{
public:
    ParserStates getState() const { return _state; }
    uint8_t getOffset() const { return _offset; }
    uint8_t *getData() const { return (uint8_t *)_data; }
    void *getUserData() const { return _user_data; }
    TelegramParsedHandler getHandler() const { return _telegramParsedHandler; }

    static const uint8_t EMPTY = TelegramParser::ParserStates::EMPTY;
    static const uint8_t TELEGRAM_START = TelegramParser::ParserStates::TELEGRAM_START;
    static const uint8_t READING_PDU = TelegramParser::ParserStates::READING_PDU;
    static const uint8_t READING_FOOTER = TelegramParser::ParserStates::READING_FOOTER;
};

/**
 * @brief Empty callback function for testing
 */
static void telegram_parsed_callback(const BaseTelegram &data, void *user_data)
{
    // Empty callback for testing
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

#pragma region Test cases
/**
 * @brief Test initial state of TelegramParser and default values
 */
void test_telegram_parser_initial_state()
{
    MockParser parser;
    TEST_ASSERT_EQUAL(MockParser::EMPTY, parser.getState());
    TEST_ASSERT_EQUAL(0, parser.getOffset());
    TEST_ASSERT_NOT_NULL(parser.getData());
    TEST_ASSERT_NULL(parser.getUserData());
    TEST_ASSERT_NULL(parser.getHandler());
}

/**
 * @brief Test storing and retrieving user data and handler
 */
void test_telegram_parser_stored_data(void)
{
    MockParser parser;

    parser.begin(telegram_parsed_callback, (void *)0x1234);

    TEST_ASSERT_EQUAL((void *)0x1234, parser.getUserData());
    TEST_ASSERT_EQUAL(telegram_parsed_callback, parser.getHandler());
}

/**
 * @brief Test flush functionality of TelegramParser
 *
 * Part of a telegram is being fed into the parser, the state is verified and
 * then flush is called to reset the parser state. The state is verified again to
 * ensure it has been reset.
 */
void test_telegram_parser_flush(void)
{
    MockParser parser;

    parser.begin(telegram_parsed_callback, (void *)0x1234);

    // Simulate some parsing
    parser.parseChunk((uint8_t *)"\xB6\x6B\x01\x02\x03\x04", 6);

    TEST_ASSERT_EQUAL(MockParser::READING_PDU, parser.getState());
    TEST_ASSERT_EQUAL(6, parser.getOffset());

    // Now flush
    parser.flush();

    TEST_ASSERT_EQUAL(MockParser::EMPTY, parser.getState());
    TEST_ASSERT_EQUAL(0, parser.getOffset());
}

/**
 * @brief Test parsing incomplete telegram does not trigger callback
 */
void test_telegram_parser_incomplete_no_callback(void)
{
    MockParser parser;
    bool callback_called = false;

    auto test_callback = [](const BaseTelegram &data, void *user_data)
    {
        bool *called = (bool *)user_data;
        *called = true;
    };

    parser.begin(test_callback, (void *)&callback_called);

    // Simulate incomplete telegram
    parser.parseChunk((uint8_t *)"\xB6\x6B\x01\x02", 4);

    TEST_ASSERT_FALSE(callback_called);
}

/**
 * @brief Test parsing incomplete telegram does not trigger callback
 */
void test_telegram_parser_incomplete_and_new_no_callback(void)
{
    MockParser parser;
    bool callback_called = false;

    auto test_callback = [](const BaseTelegram &data, void *user_data)
    {
        bool *called = (bool *)user_data;
        *called = true;
    };

    parser.begin(test_callback, (void *)&callback_called);

    // Simulate incomplete telegram
    parser.parseChunk((uint8_t *)"\xB6\x6B\x01\x02", 4);

    // Simulate start of new telegram
    parser.parseChunk((uint8_t *)"\xB6\x6B\x01\x02", 4);

    TEST_ASSERT_FALSE(callback_called);
}

/**
 * @brief Test parsing complete telegram triggers callback
 */
void test_telegram_parser_complete_telegram_callback(void)
{
    MockParser parser;
    bool callback_called = false;

    auto test_callback = [](const BaseTelegram &data, void *user_data)
    {
        bool *called = (bool *)user_data;
        *called = true;
    };

    parser.begin(test_callback, (void *)&callback_called);

    // Simulate complete telegram
    parser.parseChunk(batteryData, sizeof(batteryData));

    TEST_ASSERT_TRUE(callback_called);
}

/**
 * @brief Test that incomplete telegram is dropped when new complete
 * telegram arrives.
 *
 * We parse a chunk of an incomplete telegram first, then start with a complete
 * new one. The callback should only be triggered for the complete telegram.
 */
void test_telegram_parser_incomplete_dropped()
{
    MockParser parser;

    typedef struct
    {
        uint8_t called;
        uint8_t last_type;
    } callback_ctx;
    callback_ctx ctx = {0, 0};

    auto test_callback = [](const BaseTelegram &data, void *user_data)
    {
        callback_ctx *ctx = (callback_ctx *)user_data;

        ctx->called++;
        ctx->last_type = data.getInternalTelegramType();
    };

    parser.begin(test_callback, (void *)&ctx);

    // Simulate incomplete telegram
    parser.parseChunk(ecuData, sizeof(ecuData) - 5);

    // Simulate complete telegram (will be dropped)
    parser.parseChunk(batteryData, sizeof(batteryData));

    TEST_ASSERT_EQUAL(0, ctx.called);

    // Simulate another complete telegram
    parser.parseChunk(batteryData, sizeof(batteryData));

    TEST_ASSERT_EQUAL(1, ctx.called);
    TEST_ASSERT_EQUAL(BatteryStatus::TELEGRAM_TYPE_BATTERY_STATUS, ctx.last_type);
}

/**
 * @brief Feed a valid telegram byte by byte and verify the parser status
 */
void test_telegram_parser_states(void)
{

    MockParser parser;
    parser.begin(telegram_parsed_callback, nullptr);

    size_t len = sizeof(batteryData) / sizeof(batteryData[0]);

    for (size_t i = 0; i < len; ++i)
    {
        parser.parseChunk(&batteryData[i], 1);
        uint8_t offset = parser.getOffset();

        if (offset < 2)
        {
            TEST_ASSERT_EQUAL(MockParser::EMPTY, parser.getState());
        }
        else if (offset < 5)
        {
            TEST_ASSERT_EQUAL(MockParser::TELEGRAM_START, parser.getState());
        }
        else if (offset >= 5 && offset < len - 2)
        {
            TEST_ASSERT_EQUAL(MockParser::READING_PDU, parser.getState());
        }
        else
        {
            TEST_ASSERT_EQUAL(MockParser::READING_FOOTER, parser.getState());
        }
    }

    TEST_ASSERT_EQUAL(MockParser::EMPTY, parser.getState());
}
#pragma endregion

void run_telegram_parser_tests()
{
    RUN_TEST(test_telegram_parser_stored_data);
    RUN_TEST(test_telegram_parser_flush);
    RUN_TEST(test_telegram_parser_incomplete_no_callback);
    RUN_TEST(test_telegram_parser_incomplete_and_new_no_callback);
    RUN_TEST(test_telegram_parser_complete_telegram_callback);
    RUN_TEST(test_telegram_parser_incomplete_dropped);
    RUN_TEST(test_telegram_parser_states);
}
