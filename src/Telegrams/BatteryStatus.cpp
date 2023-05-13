
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
        if (this->_pduLen != TELEGRAM_SIZE)
        {
            this->_isValid = false;
            return;
        }
    }

    /**
     * @brief Get string representation of the object
     */
    String BatteryStatus::toString()
    {
        String s = "Battery Status: ";
        s.concat(getVoltage());
        s += "V ,";
        s.concat(getSoC());
        s += "%, ";
        s.concat(getTemperature());
        s += "°C, ";
        s.concat(getChargeCurrent());
        s += "Amp, ";
        s.concat(getCycles());
        s += "x, ";
        s += "Charging: " + isCharging() ? "true" : "false";

        return s;
    }

    /**
     * @brief Get a detailed string representation of the object
     */
    String BatteryStatus::toStringDetailed()
    {
        String s = BaseTelegram::toString();
        s += " -> ";
        s += toString();
        return s;
    }

    /**
     * @brief Current charge or discharge current
     */
    double BatteryStatus::getChargeCurrent()
    {
        double val = _pdu[POS_CHARGE];
        if (val >= 100)
            val /= 10.0;
        return val;
    }

    /**
     * @brief Get charging state of the BMS
     */
    bool BatteryStatus::isCharging()
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