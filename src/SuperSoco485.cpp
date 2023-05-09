#include "SuperSoco485.h"
#include <Arduino.h>

/**
 * @brief Create a new instance of the SuperSoco485 class
 * 
*/
SuperSoco485::SuperSoco485()
{

}

/**
 * @brief Initialize the hardware facilities
 */
void SuperSoco485::begin()
{

}

/** @brief Set the telegram callback
 * @param callback Callback to register for parsed telegrams
 */
void SuperSoco485::setTelegramReceived(ParsedTelegramReceived callback)
{
    if (callback != NULL)
    {
        this->_callback = callback;
    }
}

/**
 * @brief Handle the serial RS485 interface
 * Reads all received data from the serial interface, parses the bytes as 
 * telegrams and calls the callback function for each received and parsed
 * telegram.
*/
void SuperSoco485::update()
{
}