#include <assert.h>
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
        : _telegramParsedHandler(NULL), _user_data(NULL)
    {
    }

    void TelegramParser::begin(TelegramParsedHandler telegramParsed,
                               void *user_data = NULL)
    {
        assert(telegramParsed != NULL);
        this->_telegramParsedHandler = telegramParsed;
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
            uint8_t b = raw[i];
            switch (_state)
            {
            case EMPTY:
                if (_offset == 0 && b == FIRST_BYTE_RESPONSE || b == FIRST_BYTE_REQUEST)
                {
                    _data[_offset++] = b;
                }
                else if (_offset == 1 && (b == SECOND_BYTE_RESPONSE || b == SECOND_BYTE_REQUEST))
                {
                    // first byte was received and second byte matches ->
                    // Continue to reading telegram data
                    _data[_offset++] = b;
                    _state = TELEGRAM_START;
                }
                else
                {
                    // Not a correct start -> reset buffer
                    _offset = 0;
                }
                break;

            case TELEGRAM_START:
                // A valid start has been received. Append all data to the
                // buffer until we have received a sane PDU length.
                _data[_offset++] = b;

                // PDU length is at position 4 (offset must already be set to 5)
                if (_offset == POS_PDU_LENGTH + 1)
                {
                    // sanity check of length
                    if (b > MAX_PDU_LENGTH)
                    {
                        // not a sane PDU length, reset parser
                        flush();
                    }
                    else
                    {
                        // valid length received, continue reading PDU
                        _state = READING_PDU;
                    }
                }
                break;

            case READING_PDU:
            {
                uint8_t pduLen = _data[POS_PDU_LENGTH];
                _data[_offset++] = b;
                if (_offset == POS_PDU_LENGTH + pduLen + 1)
                {
                    // full PDU received, continue to footer
                    _state = READING_FOOTER;
                }
            }
            break;

            case READING_FOOTER:
            {
                uint8_t pduEnd = POS_PDU_LENGTH + _data[POS_PDU_LENGTH];
                _data[_offset++] = b;
                if (b == TELEGRAM_TERMINATOR)
                {
                    // full telegram received, finish block
                    finishBlock();
                }
                else if (_offset > pduEnd + 2)
                {
                    // After the pdu, we expect 1 byte checksum and 1 byte
                    // terminator. In this case, the terminator was not found
                    // and we cannot expect that the telegram is valid anymore.
                    // Discard current data.
                    flush();
                    _state = EMPTY;
                }
            }
            break;

            default:
                // Serial.print("Unknown state ");
                // Serial.println(_state);
                flush();
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
            // Check if telegram is valid. If not, discard it.
            if (!isTelegramValid())
            {
                flush();
                return;
            }

            // handle block
            BaseTelegram b(_data, _offset);

            // Update to specialized class
            if (b.getType() == BaseTelegram::TelegramType::RESPONSE)
            {
                if (BatteryStatus::isBatteryStatusTelegram(b))
                {
                    BatteryStatus bms(b);
#ifdef DEBUG
                    Serial.println(bms.toStringDetailed());
#endif
                    if (bms.isValid() && _telegramParsedHandler != NULL)
                    {
                        _telegramParsedHandler(bms, this->_user_data);
                    }
                }
                else if (ECUStatus::isECUStatusTelegram(b))
                {
                    ECUStatus ecu(b);

#ifdef DEBUG
                    Serial.println(ecu.toStringDetailed());
#endif
                    if (ecu.isValid() && _telegramParsedHandler != NULL)
                    {
                        _telegramParsedHandler(ecu, this->_user_data);
                    }
                }
#ifdef DEBUG
                else
                {
                    Serial.println(b.toString());
                }
#endif
            }

            // reset the buffer for the next block
            flush();
        }
    }

    /**
     * @brief Flush all unprocessed data inside the parser
     *
     * Clears the internal counter so the first incoming chunk will be
     * processed freshly
     */
    void TelegramParser::flush()
    {
        _state = EMPTY;
        _offset = 0;
    }

    /**
     * @brief Calculate the checksum of the received telegram and compare it to
     * the received checksum byte.
     * @return true if telegram is valid
     */
    bool TelegramParser::isTelegramValid()
    {
        uint8_t checksum = _data[POS_PDU_LENGTH];
        for (uint8_t i = POS_PDU_LENGTH + 1; i < _offset - 2; i++)
        {
            checksum ^= _data[i];
        }
        return (checksum == _data[_offset - 2]);
    }

}
/** @} */
