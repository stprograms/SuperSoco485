/**
 * @file ErrorCode.h
 * @brief Headerfile defining the error codes used in SuperSoco485
 */
#ifndef ERROR_CODE_H
#define ERROR_CODE_H

#include <stdint.h>
/**
 * @addtogroup error_code Error Code
 * @{
 */
namespace stprograms::SuperSoco485
{
    class ErrorCode
    {
    private:
        uint8_t code;

    public:
        /// @brief Create a new code class
        /// @param code error code type
        ErrorCode(uint8_t code) : code(code) {}

        /// @brief Get the string representation of the error code
        /// @return string representation
        const char *toString() const;

        /// @brief Get the stored error code
        const uint8_t errorCode() const { return this->code; }

        /**
         *  @brief Optional, additional detail for some codes.
         * Gives additional information on the error. Currently only used for code
         * 98, where detail states the set bit in the error byte of the controller.
         * See [error code definition](https://github.com/stprograms/SuperSoco485Monitor?tab=readme-ov-file#error-codes)
         * for further information
         */
        uint8_t detail = 0;
    };
}
/** @} */

#endif // ERROR_CODE_H
