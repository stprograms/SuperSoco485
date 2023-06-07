#include "SuperSoco485.h"
#include <Arduino.h>
#include <ArduinoRS485.h>

#include "BatteryStatus.h"
#include "ECUStatus.h"

// #define TRACE
// #define DEBUG

using namespace stprograms::SuperSoco485;

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
    : _parser(stprograms::SuperSoco485::TelegramParser(this))
{
    _parser.setBatStatusHandler(batteryStatusReceived);
    _parser.setECUStatusHandler(ecuStatusReceived);
}

/// @brief Set the data changed callback
/// @param callback callback to be called
/// @param user_data user data that will be transmitted with the callback
void SuperSoco485::setCallback(DataChangedHandler callback, void *user_data)
{
    if (callback != NULL)
    {
        this->_callback = callback;
    }

    this->_user_data = user_data;
}

/**
 * @brief Initialize the hardware facilities
 */
void SuperSoco485::begin()
{
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
                // Until(TelegramParser::TELEGRAM_TERMINATOR,
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

/// @brief A telegram has been parsed
/// @param user_data pointer to SuperSoco485 instance
/// @param data parsed telegram
void SuperSoco485::batteryStatusReceived(void *user_data, BaseTelegram *data)
{
    SuperSoco485 *ss = (SuperSoco485 *)user_data;
    BatteryStatus *status = (BatteryStatus *)data;
    bool hasChanged = false;

#ifdef DEBUG
    // Serial.println("Battery Status recv:");
    Serial.println(data->toStringDetailed());
#endif

    // compare data
    compareData(ss->_status.batVoltage, status->getVoltage(), hasChanged);
    compareData(ss->_status.Soc, status->getSoC(), hasChanged);
    compareData(ss->_status.batTemp, status->getTemperature(), hasChanged);
    compareData(ss->_status.chargeCurrent, status->getChargeCurrent(), hasChanged);
    compareData(ss->_status.chargeCycle, status->getCycles(), hasChanged);
    compareData(ss->_status.charging, status->isCharging(), hasChanged);

    // Call callback
    if (
        hasChanged &&
        ss->_callback != NULL)
    {
        ss->_callback(ss->_user_data, ss);
    }
}

/// @brief Callback for ecu status
/// @param user_data pointer to SuperSoco485 instance
/// @param data parsed telegram
void SuperSoco485::ecuStatusReceived(void *user_data, BaseTelegram *data)
{
    SuperSoco485 *ss = (SuperSoco485 *)user_data;
    ECUStatus *status = (ECUStatus *)data;

    bool hasChanged = false;

#ifdef DEBUG
    // Serial.println("ECU Status recv:");
    Serial.println(data->toStringDetailed());
#endif

    // compare data
    compareData(ss->_status.driveMode, status->getDriveMode(), hasChanged);
    compareData(ss->_status.ecuTemp, status->getTemperature(), hasChanged);
    compareData(ss->_status.parking, status->isParking(), hasChanged);
    compareData(ss->_status.current, status->getCurrent(), hasChanged);
    compareData(ss->_status.speed, status->getSpeed(), hasChanged);

    // Call callback
    if (
        hasChanged &&
        ss->_callback != NULL)
    {
        ss->_callback(ss->_user_data, ss);
    }
}
/** @} */