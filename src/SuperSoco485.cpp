#include "SuperSoco485.h"
#include <Arduino.h>
#include <ArduinoRS485.h>

using namespace stprograms::SuperSoco485;

/**
 * @addtogroup ss485
 * @{
 */

/// @brief Baudrate used for communication
const unsigned long SUPER_SOCO_BAUDRATE = 9600;
const uint8_t TELEGRAM_TERMINATOR = 0x0D;

/**
 * @brief Create a new instance of the SuperSoco485 class
 * @param callback Callback to register for parsed telegrams
 */
SuperSoco485::SuperSoco485() 
: _parser (stprograms::SuperSoco485::TelegramParser(telegramParsed, this))
{

}

void SuperSoco485::setCallback(ParsedTelegramReceived callback, void *user_data)
{
    if (callback != NULL)
    {
        this->_callback = callback;
    }
    
    this->user_data = user_data;
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
    if (RS485.available() > 0)
    {
        // read data and parse telegram
        size_t readBytes = 0;
        do
        {
            readBytes = RS485.readBytesUntil(TELEGRAM_TERMINATOR,
                                             _rawBuffer,
                                             sizeof(_rawBuffer));

            // forward data to parser
            _parser.parseChunk(_rawBuffer, readBytes);
        } while (readBytes != 0);
    }
}

void SuperSoco485::telegramParsed(void *sender, BaseTelegram *data)
{
    SuperSoco485 *ss = (SuperSoco485*) sender;
    
    // TODO: Check for changes

    // Call callback
    if(ss->_callback != NULL && data != NULL)
    {
        ss->_callback(ss->user_data, ss, data);
    }

}

/** @} */