#include <stdio.h>
#include "ECUStatus.h"

/**
 * @addtogroup ecu_stat
 * @{
 */

namespace stprograms::SuperSoco485
{

    /// @brief Copy constructor
    /// @param c Original BaseTelegram
    ECUStatus::ECUStatus(BaseTelegram &c)
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
    ECUStatus::ECUStatus(ECUStatus &c)
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
    ECUStatus::ECUStatus()
        : BaseTelegram(NULL, 0)
    {
        _telegramType = TELEGRAM_TYPE_ECU_STATUS;
        _isValid = false;
    }

    /**
     * @brief Get string representation of the object
     */
    const char *ECUStatus::toString() const
    {
        static char s[64];
        snprintf(s, sizeof(s), "ECU Status: Drive %d, %dmA, %dkm/h, %d °C, Parking: %s",
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
    const char *ECUStatus::toStringDetailed() const
    {
        static char s[128];
        snprintf(s, sizeof(s), "%s -> %s", BaseTelegram::toString(), toString());
        return s;
    }

    /**
     * @brief Get parking state of the ECU
     */
    bool ECUStatus::isParking() const
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
