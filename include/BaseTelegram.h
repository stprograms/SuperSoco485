/**
 * @file BaseTelegram.h
 * @author Andreas Kurzmann
 * @brief Headerfile of the BaseTelegram class
 */

#ifndef BASE_TELEGRAM_H
#define BASE_TELEGRAM_H

#include <Arduino.h>

/**
 * @addtogroup baseTel BaseTelegram
 * @{
 */
namespace stprograms::SuperSoco485
{
    /**
     * @brief Basic telegram structure
     */
    class BaseTelegram
    {
    public:
        enum TelegramType
        {
            READ_REQUEST = 0xC55C,
            READ_RESPONSE = 0xB66B
        };

        /// @brief Start Sequence of the telegram
        uint16_t getStart() const { return (uint16_t)((_raw[0] << 8) + _raw[1]); }

        /// @brief Source of the telegram
        byte getSource() const { return _raw[POS_SRC]; }

        /// @brief Destination of the telegram
        byte getDestination() const { return _raw[POS_DES]; }

        /// @brief User Data
        const byte *getPDU() const { return _pdu; }

        /// @brief Received checksum
        byte getChecksum() const { return (byte)(_raw[POS_LEN + _pduLen + 1]); }

        /// @brief Are the raw data valid against the Checksum
        bool isValid() const { return _isValid; }

        /// @brief Get the type of the telegram
        TelegramType getType() const { return (TelegramType)((_raw[0] << 8) + _raw[1]); }

        // constructor
        BaseTelegram(byte *rawData, size_t len);
        BaseTelegram(BaseTelegram &c);

        virtual String toString();
        virtual String toStringDetailed();

        /// @brief Equal operator
        /// @param b object to compare
        /// @return true if objects are equal
        bool operator==(BaseTelegram const &b) const
        {
            if (this->_rawLen != b._rawLen)
                return false;
            return (memcmp(this->_raw, b._raw, this->_rawLen) == 0);
        }

        /// @brief Unequal operator
        /// @param b object to compare
        /// @return true of objects are unequal
        bool operator!=(BaseTelegram const &b) const
        {
            if (this->_rawLen != b._rawLen)
                return true;
            return (memcmp(this->_raw, b._raw, this->_rawLen) != 0);
        }


    private:
        /// @brief Maximum supported data length
        static const byte MAX_DATA_LEN = 32;

    protected:
        // Constants

        /// @brief End byte of the telegram
        const byte END_TELEGRAM = 0x0D;

        /// @brief Position of the source id in the raw data
        const byte POS_SRC = 2;
        /// @brief Position of the destination id in the raw data
        const byte POS_DES = 3;
        /// @brief Position of the data length in the raw data
        const byte POS_LEN = 4;

        // Local members
        byte _raw[MAX_DATA_LEN];
        byte _pdu[MAX_DATA_LEN];

        size_t _rawLen;
        size_t _pduLen;
        bool _isValid;

        static String hexToStr(byte);
    };
}

/** @} */
#endif
