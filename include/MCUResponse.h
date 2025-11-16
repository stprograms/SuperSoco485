/**
 * @file MCUResponse.h
 * @author Andreas Kurzmann
 * @brief Headerfile of the MCUResponse class
 */

#ifndef MCU_RESPONSE_H
#define MCU_RESPONSE_H

#include "BaseTelegram.h"

/**
 * @addtogroup mcu_response MCU Response
 * @{
 */

#ifndef SUPER_SOCO_PARKING_INVERT
/**
 * @brief Invert the interpretation of parking
 *
 * Depending on the MCU, the parking state is interpreted differently.
 * If the parsed result does not match the real status of the bike,
 * set this define to != 0
 */
#define SUPER_SOCO_PARKING_INVERT 0
#endif

namespace stprograms::SuperSoco485
{
    /**
     * @brief MCU Response telegram class
     *
     * This class represents the Response of the Motor Controller Unit
     */
    class MCUResponse : public BaseTelegram
    {
    public:
        static const uint8_t TELEGRAM_TYPE_ECU_STATUS = 0x02;

        static bool isMCUResponseTelegram(BaseTelegram &baseTel)
        {
            return (baseTel.getSource() == Unit::UNIT_CONTROLLER &&
                    baseTel.getDestination() == Unit::UNIT_MASTER);
        }

        virtual const char *toString() const;
        virtual const char *toStringDetailed() const;

        /// @brief Current Drive Mode
        uint8_t getGear() const { return _pdu[POS_GEAR]; }

        /// @brief Current in mA
        uint16_t getCurrent() const { return (_pdu[POS_CURRENT_H] << 8) + _pdu[POS_CURRENT_L]; }

        /// @brief  Current speed
        uint16_t getSpeed() const { return (_pdu[POS_SPEED_H] << 8) + _pdu[POS_SPEED_L]; }

        /// @brief Current ECU temperature
        int8_t getTemperature() const { return (int8_t)_pdu[POS_TEMP]; }

        /// @brief Get parking mode
        bool isParking() const;

        // Copy constructor
        MCUResponse(BaseTelegram &c);
        MCUResponse(MCUResponse &c);

        MCUResponse();

    protected:
        // Constants
        /// @brief Required size of the telegram
        const size_t TELEGRAM_SIZE = 0x0A;

        /// @brief Position of gear in PDU
        const uint8_t POS_GEAR = 0;
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
        /// @brief Position of error code
        const uint8_t POS_ERROR_CODE = 6;
        /// @brief Position of parking information in PDU
        const uint8_t POS_PARKING = 8;
    };
}

/** @} */
#endif
