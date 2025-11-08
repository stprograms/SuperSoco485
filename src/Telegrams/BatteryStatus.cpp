#include <stdio.h>
#include "BatteryStatus.h"
/**
 * @addtogroup batStat
 * @{
 */

namespace stprograms::SuperSoco485
{

    /// @brief Copy constructor with BaseTelegram
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

    /// @brief Copy constructor with BatteryStatus
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
    const char *BatteryStatus::toString() const
    {
        static char s[64];
        snprintf(s, sizeof(s), "Battery Status: %dV, %d\%, %d°C, %d A, %dx, Charging: %s",
                 getVoltage(),
                 getSoC(),
                 getTemperature(),
                 getChargeCurrent(),
                 getLoadCycles(),
                 getActivity() == BatteryActivity::CHARGING ? "true" : "false");

        return s;
    }

    /**
     * @brief Get a detailed string representation of the object
     */
    const char *BatteryStatus::toStringDetailed() const
    {
        static char s[128];
        snprintf(s, sizeof(s), "%s -> %s", BaseTelegram::toString(), toString());
        return s;
    }

}

/** @} */
