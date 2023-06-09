/**
 * @file BatteryStatus.h
 * @author Andreas Kurzmann
 * @brief Headerfile of the BatteryStatus class
 */

#ifndef BATTERY_STATUS_H
#define BATTERY_STATUS_H

#include <Arduino.h>
#include "BaseTelegram.h"

/**
 * @addtogroup batStat BatteryStatus
 * @{
 */
namespace stprograms::SuperSoco485
{
    /**
     * @brief Basic telegram structure
     */
    class BatteryStatus : public BaseTelegram
    {
    public:
        static const uint8_t TELEGRAM_TYPE_BATTERY_STATUS = 0x01;

        virtual String toString() const;
        virtual String toStringDetailed() const;

        /// @brief Current Battery Voltage in Volts
        byte getVoltage() const { return _pdu[POS_VOLTAGE]; }

        /// @brief Current State of Charge in percent
        byte getSoC() const { return _pdu[POS_SOC]; }

        /// @brief  Current temperature of BMS in °C
        byte getTemperature() const { return _pdu[POS_TEMP]; }

        /// @brief Current charge or discharge current
        double getChargeCurrent() const;

        /// @brief Get the number of charging cycles
        uint16_t getCycles() const { return (uint16_t)((_pdu[POS_CYCLE_H] << 8) + _pdu[POS_CYCLE_L]); }

        /// @brief Get charging state of the BMS
        /// @return true on charging
        bool isCharging() const;

        // Copy constructor
        BatteryStatus(BaseTelegram &c);
        BatteryStatus(BatteryStatus &c);

        BatteryStatus();

    protected:
        // Constants
        /// @brief Required size of the telegram
        const size_t TELEGRAM_SIZE = 0x0A;

        /// @brief Position of battery voltage in PDU
        const byte POS_VOLTAGE = 0;
        /// @brief position of State of Charge in PDU
        const byte POS_SOC = 1;
        /// @brief Position of temperature in PDU
        const byte POS_TEMP = 2;
        /// @brief Position of charge / discharge current in PDU
        const byte POS_CHARGE = 3;
        /// @brief Positiion of high byte of number of charging cycles in PDU
        const byte POS_CYCLE_H = 4;
        /// @brief Position of low byte of number of charging cycles in PDU
        const byte POS_CYCLE_L = 5;
        /// @brief Position of charging information in PDU
        const byte POS_CHARGING = 9;
    };
}

/** @} */
#endif
