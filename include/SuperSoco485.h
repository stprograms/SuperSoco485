/**
 * @file SuperSoco485.h
 * @brief Main header file for the RS485 implementation of Super Soco motorcycles
 */

#ifndef SUPER_SOCO_485_H
#define SUPER_SOCO_485_H

#include <stdint.h>
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

        void begin(
            DataChangedHandler vehicleDataUpdatedHandler = NULL,
            void *user_data = NULL);
        void parseChunk(uint8_t *raw, size_t len);
        void flush();

        /// @brief Status information
        class Status
        {
        public:
            /// @brief Current battery voltage
            uint8_t batVoltage = 0;
            /// @brief State of Charge
            uint8_t Soc = 0;
            /// @brief battery temperature
            int8_t batTemp = 0;
            /// @brief charge / discharge current
            int8_t chargeCurrent = 0;
            /// @brief Number of charging cycles
            uint16_t chargeCycle = 0;
            /// @brief Currently charging
            bool charging = false;
            /// @brief Currently discharging
            bool discharging = false;

            /// @brief current drive Mode
            uint8_t driveMode = 1;
            /// @brief temperature of ECU
            int8_t ecuTemp = 0;
            /// @brief vehicle in parking mode
            bool parking = false;
            /// @brief Speed measurement of ECU
            uint16_t speed = 0;
            /// @brief Current currently used
            uint16_t current = 0;
        };

        /// @brief Status structure
        Status _status;


    protected:
        /// @brief function to call if vehicle data has been updated
        DataChangedHandler vehicleDataUpdatedHandler;
        /// @brief Userdata do transmit with callback function
        void *_user_data;
        /// @brief telegram parser instance
        stprograms::SuperSoco485::TelegramParser _parser;

        static void telegramReceived(const BaseTelegram &telegram,
            void *user_data);

    };
}

extern stprograms::SuperSoco485::SuperSoco485 SuperSoco;

/** @} */
#endif
