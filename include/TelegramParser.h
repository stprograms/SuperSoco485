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

    protected:
        static const size_t MAX_TELEGRAM_LENGTH = 64;

        static const uint8_t FIRST_BYTE_RESPONSE = 0xB6;
        static const uint8_t SECOND_BYTE_RESPONSE = 0x6B;

        static const uint8_t FIRST_BYTE_REQUEST = 0xC5;
        static const uint8_t SECOND_BYTE_REQUEST = 0x5C;

        static const uint8_t MAX_PDU_LENGTH = 32;
        static const uint8_t POS_PDU_LENGTH = 4;

        enum ParserStates
        {
            /// @brief No telegram data in buffer yet
            EMPTY,
            /// @brief Telegram start detected, reading telegram header
            TELEGRAM_START,
            /// @brief Reading telegram PDU data
            READING_PDU,
            /// @brief Reading telegram footer
            READING_FOOTER,
        };
        ParserStates _state = EMPTY;

        uint8_t _data[MAX_TELEGRAM_LENGTH] = {
            0xFF,
        };
        uint8_t _offset = 0;
        void *_user_data;
        TelegramParsedHandler _telegramParsedHandler;

        void finishBlock();

        bool isTelegramValid();
    };
}

/** @} */
#endif /*TELEGRAM_PARSER_H*/
