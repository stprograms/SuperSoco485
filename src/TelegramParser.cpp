
#include "TelegramParser.h"
#include "BatteryStatus.h"
#include "ECUStatus.h"

namespace stprograms::SuperSoco485
{
    /**
     * @addtogroup tg_parser
     * @{
     */

    /**
     * @brief Create new Telegram Parser
     * @param user_data Pointer that will be sent with the callbacks
     */
    TelegramParser::TelegramParser(void *user_data)
        : _cb(NULL), _batStatus(NULL), _ecuStatus(NULL), _user_data(user_data)
    {
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

            // Update to specialized class
            if (b.getType() == BaseTelegram::TelegramType::READ_RESPONSE)
            {
                if (b.getSource() == 0xAA && b.getDestination() == 0x5A)
                {
                    BatteryStatus bms(b);

                    if (bms.isValid() && this->_batStatus != NULL)
                    {
                        this->_batStatus(this->_user_data, &bms);
                    }
                }
                else if (b.getSource() == 0xAA && b.getDestination() == 0xDA)
                {
                    ECUStatus ecu(b);

                    if (ecu.isValid() && this->_ecuStatus != NULL)
                    {
                        this->_ecuStatus(this->_user_data, &ecu);
                    }
                }
            }

            // Move memory
            memmove(_data, _data + (_offset - 2), 2);
            _offset = 2;
        }
    }

}
/** @} */