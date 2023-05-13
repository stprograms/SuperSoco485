
#include "BaseTelegram.h"
/**
 * @addtogroup baseTel
 * @{
*/

namespace stprograms::SuperSoco485
{
    /**
     * @brief Copy constructor
     * @param c Original object
     */
    BaseTelegram::BaseTelegram(BaseTelegram &c)
    {
        memcpy(this->_raw, c._raw, sizeof(this->_raw));
        this->_rawLen = c._rawLen;

        memcpy(this->_pdu, c._pdu, sizeof(this->_pdu));
        this->_pduLen = c._pduLen;

        this->_isValid = this->_isValid;
    }

    BaseTelegram::BaseTelegram(byte *rawData, size_t len)
    {
        if (len > MAX_DATA_LEN || len < 7)
        {
            this->_isValid = false;
            return;
        }

        memcpy(this->_raw, rawData, sizeof(this->_raw));
        this->_rawLen = len;

        // fetch user data
        this->_pduLen = this->_raw[POS_LEN];
        if (this->_pduLen > MAX_DATA_LEN)
        {
            this->_isValid = false;
            return;
        }
        memcpy(this->_pdu, this->_raw + POS_LEN + 1, this->_pduLen);

        // verify checksum
        byte calcCheck = this->_pduLen;
        for (uint8_t i = 0; i < this->_pduLen; ++i)
        {
            calcCheck ^= this->_pdu[i];
        }

        this->_isValid = (calcCheck == getChecksum());

        // Optional, make outputs
    }

    /**
     * @brief Convert hex byte to string representation
     */
    String BaseTelegram::hexToStr(byte b)
    {
        char buf[3];
        sprintf(buf, "%02X", b);
        return buf;
    }

    /**
     * @brief Get string representation of the object
     */
    String BaseTelegram::toString()
    {
        String s = "";

        for (uint8_t i = 0; i < this->_rawLen; ++i)
        {
            s += hexToStr(this->_raw[i]);
            if (i < this->_rawLen - 2)
                s += " ";
        }
        return s;
    }

    /**
     * @brief Get a detailed string representation of the object
     */
    String BaseTelegram::toStringDetailed()
    {
        return toString();
    }

}

/** @} */