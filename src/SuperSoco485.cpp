#include "SuperSoco485.h"
#include <Arduino.h>
#include <ArduinoRS485.h>

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

    /// @brief Baudrate used for communication
    const unsigned long SUPER_SOCO_BAUDRATE = 9600;

    /**
     * @brief Create a new instance of the SuperSoco485 class
     */
    SuperSoco485::SuperSoco485()
    {
    }

    /**
     * @brief Initialize the hardware facilities
     */
    void SuperSoco485::begin()
    {
        _parser.begin(this);

        RS485.begin(SUPER_SOCO_BAUDRATE);
        RS485.receive();
    }

    /**
     * @brief Handle the serial RS485 interface
     * Reads all received data from the serial interface, parses the bytes as
     * telegrams and calls the callback function for each received and parsed
     * telegram.
     */
    void SuperSoco485::update()
    {
#ifdef TRACE
        Serial.print("SS485 upd| ");
#endif
        if (RS485.available() > 0)
        {
#ifdef TRACE
            Serial.print("data: ");
#endif
            // read data and parse telegram
            size_t readBytes = 0;
            do
            {
                readBytes = RS485.readBytes(
                    _rawBuffer,
                    sizeof(_rawBuffer));

#ifdef TRACE
                Serial.print(readBytes);
                Serial.print("/");
                for (size_t i = 0; i < readBytes; ++i)
                {
                    Serial.print(_rawBuffer[i], HEX);
                }
#endif

                // forward data to parser
                _parser.parseChunk(_rawBuffer, readBytes);

#ifdef TRACE
                Serial.println("");
#endif
            } while (readBytes != 0);
        }

#ifdef TRACE
        Serial.println("no data");
#endif
    }

    /// @brief Template function for comparing data. Sets the new value in current
    /// value and if the values had a different value, sets the hasChanged value to true
    /// @tparam T type of values to compare
    /// @param curVal current value to compare
    /// @param newVal new value to compare
    /// @param hasChanged value set to true if values have changed
    template <typename T>
    void compareData(T &curVal, T newVal, bool &hasChanged)
    {
        if (curVal != newVal)
        {
            curVal = newVal;
            hasChanged = true;
        }
    }

    /// @brief A new telegram has been parsed and received
    /// @param telegram The parsed telegram
    /// @param user_data registered user data
    void telegramRecevied(const BaseTelegram &telegram, void *user_data)
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
            compareData(ss->_status.charging, bat->isCharging(), hasChanged);
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
            Serial.println("Unknown Telegram");
            break;
        }

        // send update
        // Call callback
        if (hasChanged)
        {
            // Call the data updated callback
            superSocoDataUpdated();
        }
    }
}

/// @brief Global instance
stprograms::SuperSoco485::SuperSoco485 SuperSoco;

/** @} */