#include <stdio.h>
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

    BaseTelegram::BaseTelegram(uint8_t *rawData, size_t len)
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
        uint8_t calcCheck = this->_pduLen;
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
    const char* BaseTelegram::hexToStr(uint8_t b)
    {
        char buf[3];
        sprintf(buf, "%02X", b);
        return buf;
    }

    /**
     * @brief Get string representation of the object
     */
    const char* BaseTelegram::toString() const
    {
        char s[128] = "";

        for (uint8_t i = 0; i < this->_rawLen; ++i)
        {
            strncat(s, hexToStr(this->_raw[i]), sizeof(s) - strlen(s));

            if (i < this->_rawLen - 1 && (sizeof(s) - strlen(s) > 1))
                strcat(s, " ");
        }
        return s;
    }

    /**
     * @brief Get a detailed string representation of the object
     */
    const char* BaseTelegram::toStringDetailed() const
    {
        return toString();
    }

}

/** @} */
