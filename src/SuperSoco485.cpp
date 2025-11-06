#include "SuperSoco485.h"

#include "BatteryStatus.h"
#include "ECUStatus.h"

// #define TRACE
// #define DEBUG

namespace stprograms::SuperSoco485
{

    /**
     * @addtogroup ss485
     * @{
     */

    /** @brief Baudrate used for communication */
    const unsigned long SUPER_SOCO_BAUDRATE = 9600;

    /**
     * @brief Create a new instance of the SuperSoco485 class
     */
    SuperSoco485::SuperSoco485()
        : vehicleDataUpdatedHandler(NULL),
          _user_data(NULL)
    {
    }

    /**
     * @brief Initialize the hardware facilities
     */
    void SuperSoco485::begin(
        DataChangedHandler vehicleDataUpdatedHandler,
        void *user_data)
    {
        this->vehicleDataUpdatedHandler = vehicleDataUpdatedHandler;
        this->_user_data = user_data;
        _parser.begin(SuperSoco485::telegramReceived, this);
    }

    /**
     * @brief Parse the given chunk of raw data
     * @param raw Pointer to raw data
     * @param len Number of bytes in raw data
     */
    void SuperSoco485::parseChunk(uint8_t *raw, size_t len)
    {
        _parser.parseChunk(raw, len);
    }

    /**
     * @brief Flush the internal parser
     * This will discard all unprocessed data. The next data chunk will be
     * interpreted as start of a new telegram
     */
    void SuperSoco485::flush()
    {
        _parser.flush();
    }

    /**
     * @brief Template function for comparing data.
     * @tparam T type of values to compare
     * @param curVal current value to compare
     * @param newVal new value to compare
     * @param hasChanged value set to true if values have changed
     *
     * Sets the new value in current value and if the values had a different
     * value, sets the hasChanged value to true
     */
    template <typename T>
    void compareData(T &curVal, T newVal, bool &hasChanged)
    {
        if (curVal != newVal)
        {
            curVal = newVal;
            hasChanged = true;
        }
    }

    /**
     * @brief A new telegram has been parsed and received
     * @param telegram The parsed telegram
     * @param user_data registered user data
     */
    void SuperSoco485::telegramReceived(const BaseTelegram &telegram, void *user_data)
    {
        SuperSoco485 *ss = (SuperSoco485 *)user_data;

        bool hasChanged = false;

#ifdef DEBUG
        Serial.println(telegram.toStringDetailed());
#endif

        switch (telegram.getInternalTelegramType())
        {
        case BatteryStatus::TELEGRAM_TYPE_BATTERY_STATUS:
        {
            const BatteryStatus *bat = (const BatteryStatus *)&telegram;

            // compare data and update internal status
            compareData(ss->_status.batVoltage, bat->getVoltage(), hasChanged);
            compareData(ss->_status.Soc, bat->getSoC(), hasChanged);
            compareData(ss->_status.batTemp, bat->getTemperature(), hasChanged);
            compareData(ss->_status.chargeCurrent, bat->getChargeCurrent(), hasChanged);
            compareData(ss->_status.chargeCycle, bat->getCycles(), hasChanged);
            compareData(ss->_status.charging,
                        bat->getActivity() == BatteryActivity::CHARGING,
                        hasChanged);
            compareData(ss->_status.discharging,
                        bat->getActivity() == BatteryActivity::DISCHARGING,
                        hasChanged);
        }
        break;

        case ECUStatus::TELEGRAM_TYPE_ECU_STATUS:
        {
            const ECUStatus *status = (const ECUStatus *)&telegram;

            // compare data and update data
            compareData(ss->_status.driveMode, status->getDriveMode(), hasChanged);
            compareData(ss->_status.ecuTemp, status->getTemperature(), hasChanged);
            compareData(ss->_status.parking, status->isParking(), hasChanged);
            compareData(ss->_status.current, status->getCurrent(), hasChanged);
            compareData(ss->_status.speed, status->getSpeed(), hasChanged);
        }
        break;

        default:
            break;
        }

        // send update to application if registered
        if (hasChanged && ss->vehicleDataUpdatedHandler != NULL)
        {
            // Call the data updated callback
            ss->vehicleDataUpdatedHandler(ss->_user_data, ss);
        }
    }
}

/// @brief Global instance
stprograms::SuperSoco485::SuperSoco485 SuperSoco;

/** @} */
