/**
 * @file ECUStatus.h
 * @author Andreas Kurzmann
 * @brief Headerfile of the ECUStatus class
 */

#ifndef ECU_STATUS_H
#define ECU_STATUS_H

#include "BaseTelegram.h"

/**
 * @addtogroup ecu_stat ECUStatus
 * @{
 */
namespace stprograms::SuperSoco485
{
    /**
     * @brief ECUStatus telegram class
     */
    class ECUStatus : public BaseTelegram
    {
    public:
        static const uint8_t TELEGRAM_TYPE_ECU_STATUS = 0x02;

        virtual const char* toString() const;
        virtual const char* toStringDetailed() const;

        /// @brief Current Drive Mode
        uint8_t getDriveMode() const { return _pdu[POS_MODE]; }

        /// @brief Current in mA
        uint16_t getCurrent() const { return (_pdu[POS_CURRENT_H] << 8) + _pdu[POS_CURRENT_L]; }

        /// @brief  Current speed
        uint16_t getSpeed() const { return (_pdu[POS_SPEED_H] << 8) + _pdu[POS_SPEED_L]; }

        /// @brief Current ECU temperature
        int8_t getTemperature() const { return (int8_t)_pdu[POS_TEMP]; }

        /// @brief Get parking mode
        bool isParking() const;

        // Copy constructor
        ECUStatus(BaseTelegram &c);
        ECUStatus(ECUStatus &c);

        ECUStatus();

    protected:
        // Constants
        /// @brief Required size of the telegram
        const size_t TELEGRAM_SIZE = 0x0A;

        /// @brief Position of drive mode in PDU
        const uint8_t POS_MODE = 0;
        /// @brief position of high byte of curren in PDU
        const uint8_t POS_CURRENT_H = 1;
        /// @brief Position of low byte of current in PDU
        const uint8_t POS_CURRENT_L = 2;
        /// @brief Position of high byte of speed in PDU
        const uint8_t POS_SPEED_H = 3;
        /// @brief Positiion of low byte of speed in PDU
        const uint8_t POS_SPEED_L = 4;
        /// @brief Position of temperature in PDU
        const uint8_t POS_TEMP = 5;
        /// @brief Position of parking information in PDU
        const uint8_t POS_PARKING = 8;
    };
}

/** @} */
#endif
