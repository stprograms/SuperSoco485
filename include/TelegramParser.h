#ifndef TELEGRAM_PARSER_H
#define TELEGRAM_PARSER_H

#include "BaseTelegram.h"

namespace stprograms::SuperSoco485
{
    typedef void (*TelegramParsedHandler)(const BaseTelegram &data, void *user_data);

    /**
     * @addtogroup tg_parser Telegram Parser
     * @{
     */
    class TelegramParser
    {
    public:
        /// @brief Terminator of telegrams
        static const uint8_t TELEGRAM_TERMINATOR = 0x0D;

        TelegramParser();
        void begin(TelegramParsedHandler telegramParsed, void *user_data);
        void parseChunk(uint8_t *raw, size_t len);

        void flush();

    private:
        static const size_t MAX_TELEGRAM_LENGTH = 64;

        static const uint8_t READ_FIRST_BYTE = 0xB6;
        static const uint8_t READ_SECOND_BYTE = 0x6B;

        static const uint8_t WRITE_FIRST_BYTE = 0xC5;
        static const uint8_t WRITE_SECOND_BYTE = 0x5C;

        enum States
        {
            NO_BLOCK,
            FIRST_BYTE,
            READING_BLOCK
        };
        States _state = NO_BLOCK;

        uint8_t _data[MAX_TELEGRAM_LENGTH];
        uint8_t _offset = 0;
        void *_user_data;
        TelegramParsedHandler _telegramParsedHandler;


        void finishBlock();
    };
}

/** @} */
#endif /*TELEGRAM_PARSER_H*/
