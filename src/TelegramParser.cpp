
#include "TelegramParser.h"
#include "BatteryStatus.h"
#include "ECUStatus.h"

// #define DEBUG
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
    TelegramParser::TelegramParser()
        : _user_data(NULL)
    {
    }

    void TelegramParser::begin(void *user_data)
    {
        this->_user_data = user_data;
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
#ifdef DEBUG
                    Serial.println(bms.toStringDetailed());
#endif
                    if (bms.isValid())
                    {
                        telegramRecevied(bms, this->_user_data);
                    }
                }
                else if (b.getSource() == 0xAA && b.getDestination() == 0xDA)
                {
                    ECUStatus ecu(b);

#ifdef DEBUG
                    Serial.println(ecu.toStringDetailed());
#endif
                    if (ecu.isValid())
                    {
                        telegramRecevied(ecu, this->_user_data);
                    }
                }
#ifdef DEBUG
                else
                {
                    Serial.println(b.toString());
                }
#endif
            }

            // Move memory
            memmove(_data, _data + (_offset - 2), 2);
            _offset = 2;
        }
    }

}
/** @} */