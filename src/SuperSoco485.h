/** 
 * @file SuperSoco485.h
 * @brief Main header file for the RS485 implementation of Super Soco motorcycles
 */

#ifndef SUPER_SOCO_485_H
#define SUPER_SOCO_485_H

/**
 * @addtogroup ss485 SuperSoco485
 * @{
*/

/// @brief Callback definition on parsed telegram
typedef void (*ParsedTelegramReceived)(void *sender, void *telegram);

/// @brief Basic class for SuperSoco485
class SuperSoco485
{
public:
    SuperSoco485();

    void begin();

    void setTelegramReceived(ParsedTelegramReceived callback);
    void update();

private:
    ParsedTelegramReceived _callback;
};
/** @} */
#endif