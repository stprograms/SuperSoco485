/**
 * @file SuperSoco485.h
 * @brief Main header file for the RS485 implementation of Super Soco motorcycles
 */

#ifndef SUPER_SOCO_485_H
#define SUPER_SOCO_485_H

#include "TelegramParser.h"

namespace stprograms::SuperSoco485
{
    /**
     * @addtogroup ss485 SuperSoco485
     * @{
     */

    // forward declaration
    class SuperSoco485;

    /// @brief Callback definition on parsed telegram
    typedef void (*DataChangedHandler)(void *user_data, SuperSoco485 *sender);

    /// @brief Basic class for SuperSoco485
    class SuperSoco485
    {
    public:
        SuperSoco485();

        void setCallback(DataChangedHandler callback, void *user_data);
        void begin();
        void update();

        /// @brief Status information
        class Status
        {
        public:
            /// @brief Current battery voltage
            byte batVoltage;
            /// @brief State of Charge
            byte Soc;
            /// @brief battery temperature
            byte batTemp;
            /// @brief charge / discharge current
            double chargeCurrent;
            /// @brief Number of charging cycles
            uint16_t chargeCycle;
            /// @brief Currently charging
            bool charging;
        };

        /// @brief Status structure
        Status _status;

    private:
        /// @brief function to call if data has changed
        DataChangedHandler _callback;
        /// @brief Userdata do transmit with callback function
        void *_user_data;
        /// @brief buffer of raw data for RS485
        uint8_t _rawBuffer[256];
        /// @brief telegram parser instance
        stprograms::SuperSoco485::TelegramParser _parser;

        static void batteryStatusReceived(void *sender, BaseTelegram *data);
    };
}

/** @} */
#endif
