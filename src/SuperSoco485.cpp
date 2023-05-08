#include "SuperSoco485.h"


SuperSoco485::SuperSoco485()
{

}

void SuperSoco485::begin()
{

}

/** @brief Set the telegram callback
 * 
 */
void SuperSoco485::setTelegramReceived(ParsedTelegramReceived callback)
{
    if (callback != null)
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
void SuperSoco485::handleData()
{
}