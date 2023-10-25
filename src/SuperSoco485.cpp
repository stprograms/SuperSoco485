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
        int availableBytes;
#ifdef TRACE
        Serial.print("SS485 upd| ");
#endif
        availableBytes = RS485.available();
        if (availableBytes > 0)
        {
#ifdef TRACE
            Serial.print("data: ");
#endif
            // read data and parse telegram
            // FIX #7: read only as much bytes as available. If more bytes should
            // be read than currently available, the function will still return
            // only as much bytes as available, but the function will block for
            // the default timeout of 1 second and wait for more characters.
            // This will break timing of the application
            const int bytesToRead = ((unsigned)availableBytes < sizeof(_rawBuffer)) ?
                                    availableBytes : sizeof(_rawBuffer);
            size_t readBytes = RS485.readBytes(
                _rawBuffer,
                bytesToRead);

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
        }

#ifdef TRACE
        Serial.println("no data");
#endif
    }

    /**
     * @brief set the module in standby
     *
     * Disables the receive drivers on the RS485 module and flushes all
     * unprocessed data from the internal parser
     */
    void SuperSoco485::standby()
    {
        RS485.noReceive();

        while (RS485.available()) {
            RS485.read();
        }
        _parser.flush();
    }

    /**
     * @brief Wakeup receiver again
     *
     * Enables receive drivers on the RS485 module after previous standby
     */
    void SuperSoco485::wakeup()
    {
        RS485.receive();
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
