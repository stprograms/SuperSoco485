
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
    String ECUStatus::toString() const
    {
        String s = "ECU Status: Drive ";
        s.concat(getDriveMode());
        s += ", ";
        s.concat(getCurrent());
        s += "mA, ";
        s.concat(getSpeed());
        s += "km/h, ";
        s.concat(getTemperature());
        s += " °C, ";
        s += "Parking: ";
        s += isParking() ? "true" : "false";

        return s;
    }

    /**
     * @brief Get a detailed string representation of the object
     */
    String ECUStatus::toStringDetailed() const
    {
        String s = BaseTelegram::toString();
        s += " -> ";
        s += toString();
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
            Serial.print("Parking: 0x");
            Serial.println(hexToStr(_pdu[POS_PARKING]));
            break;
        }
        return val;
    }

}

/** @} */