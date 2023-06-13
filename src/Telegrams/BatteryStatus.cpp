
#include "BatteryStatus.h"
/**
 * @addtogroup batStat
 * @{
 */

namespace stprograms::SuperSoco485
{

    /// @brief Copy constructor
    /// @param c Original BaseTelegram
    BatteryStatus::BatteryStatus(BaseTelegram &c)
        : BaseTelegram(c)
    {
        _telegramType = TELEGRAM_TYPE_BATTERY_STATUS;
        if (this->_pduLen != TELEGRAM_SIZE)
        {
            this->_isValid = false;
            return;
        }
    }
    /// @brief Copy constructor
    /// @param c Original BaseTelegram
    BatteryStatus::BatteryStatus(BatteryStatus &c)
        : BaseTelegram(c)
    {
        _telegramType = TELEGRAM_TYPE_BATTERY_STATUS;
        if (this->_pduLen != TELEGRAM_SIZE)
        {
            this->_isValid = false;
            return;
        }
    }

    /// @brief Empty, dummy constructor
    BatteryStatus::BatteryStatus()
        : BaseTelegram(NULL, 0)
    {
        _telegramType = TELEGRAM_TYPE_BATTERY_STATUS;
        _isValid = false;
    }

    /**
     * @brief Get string representation of the object
     */
    String BatteryStatus::toString() const
    {
        String s = "Battery Status: ";
        s.concat(getVoltage());
        s += "V, ";
        s.concat(getSoC());
        s += "%, ";
        s.concat(getTemperature());
        s += "°C, ";
        s.concat(getChargeCurrent());
        s += " A, ";
        s.concat(getCycles());
        s += "x, ";
        s += "Charging: ";
        s += isCharging() ? "true" : "false";

        return s;
    }

    /**
     * @brief Get a detailed string representation of the object
     */
    String BatteryStatus::toStringDetailed() const
    {
        String s = BaseTelegram::toString();
        s += " -> ";
        s += toString();
        return s;
    }

    /**
     * @brief Current charge or discharge current
     */
    double BatteryStatus::getChargeCurrent() const
    {
        double val = _pdu[POS_CHARGE];
        if (val >= 100)
            val /= 10.0;
        return val;
    }

    /**
     * @brief Get charging state of the BMS
     */
    bool BatteryStatus::isCharging() const
    {
        bool val = false;
        switch (_pdu[POS_CHARGING])
        {
        case 0:
            val = false;
            break;

        case 1:
            val = true;
            break;

        default:
            Serial.print("Charging: 0x");
            Serial.println(hexToStr(_pdu[POS_CHARGING]));
            break;
        }
        return val;
    }

}

/** @} */