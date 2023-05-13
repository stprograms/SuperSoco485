/**
 * @file SuperSoco485.h
 * @brief Main header file for the RS485 implementation of Super Soco motorcycles
 */

#ifndef SUPER_SOCO_485_H
#define SUPER_SOCO_485_H

#include "TelegramParser.h"

namespace stprograms::SuperSoco485
{
    /**
     * @addtogroup ss485 SuperSoco485
     * @{
     */

    /// @brief Callback definition on parsed telegram
    typedef void (*ParsedTelegramReceived)(void* user_data, void *sender, BaseTelegram *telegram);

    /// @brief Basic class for SuperSoco485
    class SuperSoco485
    {
    public:
        SuperSoco485();

        void setCallback(ParsedTelegramReceived callback, void *user_data);
        void begin();
        void update();

    private:
        ParsedTelegramReceived _callback;
        uint8_t _rawBuffer[256];
        stprograms::SuperSoco485::TelegramParser _parser;
        void *user_data;

        static void telegramParsed(void *sender, BaseTelegram *data);
    };
}

/** @} */
#endif
