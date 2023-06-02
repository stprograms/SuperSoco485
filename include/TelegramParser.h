#ifndef TELEGRAM_PARSER_H
#define TELEGRAM_PARSER_H

#include <Arduino.h>
#include "BaseTelegram.h"

namespace stprograms::SuperSoco485
{
    /**
     * @addtogroup tg_parser Telegram Parser
     * @{
     */
    class TelegramParser
    {
    public:
        /// @brief Terminator of telegrams
        static const uint8_t TELEGRAM_TERMINATOR = 0x0D;

        typedef void (*TelegramParsedHandler)(void *user_data, BaseTelegram *data);

        TelegramParser(void *user_data);
        void parseChunk(uint8_t *raw, size_t len);

        void setBatStatusHandler(TelegramParsedHandler handler)
        {
            this->_batStatus = handler;
        }

        void setECUStatusHandler(TelegramParsedHandler handler)
        {
            this->_ecuStatus = handler;
        }

    private:
        static const size_t MAX_TELEGRAM_LENGTH = 64;

        static const byte READ_FIRST_BYTE = 0xB6;
        static const byte READ_SECOND_BYTE = 0x6B;

        static const byte WRITE_FIRST_BYTE = 0xC5;
        static const byte WRITE_SECOND_BYTE = 0x5C;

        enum States
        {
            NO_BLOCK,
            FIRST_BYTE,
            READING_BLOCK
        };
        States _state = NO_BLOCK;

        byte _data[MAX_TELEGRAM_LENGTH];
        byte _offset = 0;
        TelegramParsedHandler _cb = NULL;
        TelegramParsedHandler _batStatus = NULL;
        TelegramParsedHandler _ecuStatus = NULL;
        void *_user_data;

        void finishBlock();
    };
}

/** @} */
#endif /*TELEGRAM_PARSER_H*/