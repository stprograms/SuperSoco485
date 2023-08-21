
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
        s += getActivity() == BatteryActivity::CHARGING ? "true" : "false";

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

}

/** @} */
