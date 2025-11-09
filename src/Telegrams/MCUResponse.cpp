#include <stdio.h>
#include "MCUResponse.h"

/**
 * @addtogroup mcu_response
 * @{
 */

namespace stprograms::SuperSoco485
{

    /// @brief Copy constructor
    /// @param c Original BaseTelegram
    MCUResponse::MCUResponse(BaseTelegram &c)
        : BaseTelegram(c)
    {
        _telegramType = TELEGRAM_TYPE_ECU_STATUS;
        if (this->_pduLen != TELEGRAM_SIZE)
        {
            this->_isValid = false;
            return;
        }
    }

    /// @brief Copy constructor
    /// @param c Original BaseTelegram
    MCUResponse::MCUResponse(MCUResponse &c)
        : BaseTelegram(c)
    {
        _telegramType = TELEGRAM_TYPE_ECU_STATUS;
        if (this->_pduLen != TELEGRAM_SIZE)
        {
            this->_isValid = false;
            return;
        }
    }

    /// @brief Empty, dummy constructor
    MCUResponse::MCUResponse()
        : BaseTelegram(NULL, 0)
    {
        _telegramType = TELEGRAM_TYPE_ECU_STATUS;
        _isValid = false;
    }

    /**
     * @brief Get string representation of the object
     */
    const char *MCUResponse::toString() const
    {
        static char s[64];
        snprintf(s, sizeof(s), "MCU Response: Drive %d, %dmA, %dkm/h, %d °C, Parking: %s",
                 getDriveMode(),
                 getCurrent(),
                 getSpeed(),
                 getTemperature(),
                 isParking() ? "true" : "false");

        return s;
    }

    /**
     * @brief Get a detailed string representation of the object
     */
    const char *MCUResponse::toStringDetailed() const
    {
        static char s[128];
        snprintf(s, sizeof(s), "%s -> %s", BaseTelegram::toString(), toString());
        return s;
    }

    /**
     * @brief Get parking state of the MCU
     */
    bool MCUResponse::isParking() const
    {
        bool val = false;
        switch (_pdu[POS_PARKING])
        {
        case 1:
            val = false;
            break;

        case 2:
            val = true;
            break;

        default:
            break;
        }
        return val;
    }

}

/** @} */
