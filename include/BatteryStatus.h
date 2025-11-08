/**
 * @file BatteryStatus.h
 * @author Andreas Kurzmann
 * @brief Headerfile of the BatteryStatus class
 */

#ifndef BATTERY_STATUS_H
#define BATTERY_STATUS_H

#include "BaseTelegram.h"

/**
 * @addtogroup batStat BatteryStatus
 * @{
 */
namespace stprograms::SuperSoco485
{
    /// @brief Possible values of the Battery Activity field (charging, discharging)
    enum BatteryActivity
    {
        UNKNOWN_ACTIVITY = 0x00,
        CHARGING = 0x01,
        DISCHARGING = 0x04,
    };

    /// @brief Values of the VBreaker field
    enum VBreaker
    {
        /// @brief Unknown breaker value
        UNKNOWN_BREAKER = 0x00,
        /// @brief BMS Stopped charging
        BMS_CHARGE_STOPPED = 0x01,
        /// @brief the charge current was too high
        CHARGE_CURRENT_TOO_HIGH = 0x02,
        /// @brief Discharge current was too high
        DISCHARGE_CURRENT_TOO_HIGH = 0x04
    };

    /**
     * @brief Basic telegram structure
     */
    class BatteryStatus : public BaseTelegram
    {
    public:
        static const uint8_t TELEGRAM_TYPE_BATTERY_STATUS = 0x01;

        /**
         * @brief Check if a BaseTelegram is a BatteryStatus telegram
         * @param baseTel BaseTelegram to check
         * @return true if telegram is BatteryStatus telegram
         */
        static bool isBatteryStatusTelegram(BaseTelegram &baseTel)
        {
            return (baseTel.getSource() == Unit::UNIT_BATTERY &&
                    baseTel.getDestination() == Unit::UNIT_MASTER);
        }

        virtual const char *toString() const;
        virtual const char *toStringDetailed() const;

        /// @brief Current Battery Voltage in Volts
        uint8_t getVoltage() const { return _pdu[POS_VOLTAGE]; }

        /// @brief Current State of Charge in percent
        uint8_t getSoC() const { return _pdu[POS_SOC]; }

        /// @brief  Current temperature of BMS in °C
        int8_t getTemperature() const { return (int8_t)_pdu[POS_TEMP]; }

        /// @brief Current charge or discharge current in Amps
        int8_t getChargeCurrent() const { return (int8_t)_pdu[POS_CHARGE]; }

        /// @brief Get the number of charging cycles
        uint16_t getLoadCycles() const { return (uint16_t)((_pdu[POS_CYCLE_H] << 8) + _pdu[POS_CYCLE_L]); }

        /// @brief Get the number of discharge cycles
        uint16_t getDischargeCycles() const { return (uint16_t)((_pdu[POS_DISCHARGE_CYCLE_H] << 8) + _pdu[POS_DISCHARGE_CYCLE_L]); }

        /** @brief Battery error code.
         * TODO: Replace with enum when error codes later
         */
        uint8_t getErrorCode() const { return _pdu[POS_ERROR_CODE]; }

        /// @brief Get charging state of the BMS
        /// @return Battery Activity
        BatteryActivity getActivity() const { return (BatteryActivity)_pdu[POS_CHARGING]; }

        // Copy constructor
        BatteryStatus(BaseTelegram &c);
        BatteryStatus(BatteryStatus &c);

        BatteryStatus();

    protected:
        // Constants
        /// @brief Required size of the telegram
        const size_t TELEGRAM_SIZE = 0x0A;

        /// @brief Position of battery voltage in PDU
        const uint8_t POS_VOLTAGE = 0;
        /// @brief position of State of Charge in PDU
        const uint8_t POS_SOC = 1;
        /// @brief Position of temperature in PDU
        const uint8_t POS_TEMP = 2;
        /// @brief Position of charge / discharge current in PDU
        const uint8_t POS_CHARGE = 3;
        /// @brief Positiion of high byte of number of charging cycles in PDU
        const uint8_t POS_CYCLE_H = 4;
        /// @brief Position of low byte of number of charging cycles in PDU
        const uint8_t POS_CYCLE_L = 5;
        /// @brief Position of high byte of number of discharging cycles in PDU
        const uint8_t POS_DISCHARGE_CYCLE_H = 6;
        /// @brief Position of low byte of number of discharging cycles in PDU
        const uint8_t POS_DISCHARGE_CYCLE_L = 7;
        /// @brief Position of error code in PDU
        const uint8_t POS_ERROR_CODE = 8;
        /// @brief Position of VBreaker information in PDU
        const uint8_t POS_VBREAKER = 8;
        /// @brief Position of charging information in PDU
        const uint8_t POS_CHARGING = 9;
    };
}

/** @} */
#endif
