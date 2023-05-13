
#include "TelegramParser.h"
#include "BatteryStatus.h"

namespace stprograms::SuperSoco485
{
    /**
     * @addtogroup tg_parser
     * @{
     */

    /**
     * @brief Create new Telegram Parser
     */
    TelegramParser::TelegramParser(TelegramParsed handler, void *user_data)
    {
        this->_cb = handler;
        this->user_data = user_data;
    }

    /**
     * @brief Parse a received telegram
     * @param raw Raw data of telegram
     * @param len Length of the raw buffer
     */
    void TelegramParser::parseChunk(uint8_t *raw, size_t len)
    {
        for (size_t i = 0; i < len; ++i)
        {
            byte b = raw[i];
            switch (_state)
            {
            case NO_BLOCK:
                if (b == READ_FIRST_BYTE || b == WRITE_FIRST_BYTE)
                {
                    _data[_offset++] = b;
                    _state = FIRST_BYTE;
                }
                break;

            case FIRST_BYTE:
                _data[_offset++] = b;
                if (b == READ_SECOND_BYTE || b == WRITE_SECOND_BYTE)
                {
                    finishBlock();
                    _state = READING_BLOCK;
                }
                else if (_offset != 0)
                {
                    _state = READING_BLOCK;
                }
                else
                {
                    _state = NO_BLOCK;
                    _offset = 0;
                }
                break;

            case READING_BLOCK:
                _data[_offset++] = b;
                if (b == READ_FIRST_BYTE || b == WRITE_FIRST_BYTE)
                {
                    _state = FIRST_BYTE;
                }
                break;

            default:
                Serial.print("Unknown state ");
                Serial.println(_state);
                break;
            }
        }
    }

    /// <summary>
    /// Finishes the current block, converts the raw data and updates the buffer
    /// to continue with a new block
    /// </summary>
    void TelegramParser::finishBlock()
    {
        // finish previous block
        if (_offset > 2)
        {
            // handle block
            BaseTelegram b(_data, _offset - 2);
            BaseTelegram *special = NULL;

            // Update to specialized class
            if (b.getType() == BaseTelegram::TelegramType::READ_RESPONSE)
            {
                if (b.getSource() == 0xAA && b.getDestination() == 0x5A)
                {
                    special = new BatteryStatus(b);
                }
                else if (b.getSource() == 0xAA && b.getDestination() == 0xDA)
                {
                    // TODO: create ECU Status
                    //special = new ECUStatus(b);
                }
            }

            // Call the callback function for special telegrams
            if (special != NULL && special->isValid() == true && _cb != NULL)
            {
                _cb(this->user_data, special);
            }

            // delete the specialization
            if (special != &b && special != NULL)
            {
                delete special;
                special = NULL;
            }

            // Move memory
            memmove(_data, _data + (_offset - 2), 2);
            _offset = 2;
        }
    }

}
/** @} */