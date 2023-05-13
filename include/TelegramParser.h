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
        typedef void (*TelegramParsed)(void *user_data, BaseTelegram *data);

        TelegramParser(TelegramParsed handler, void *user_data);
        void parseChunk(uint8_t *raw, size_t len);

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
        TelegramParsed _cb;
        void *user_data;

        void finishBlock();
    };
}

/** @} */
#endif /*TELEGRAM_PARSER_H*/