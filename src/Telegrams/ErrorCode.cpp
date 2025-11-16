#include "ErrorCode.h"

using namespace stprograms::SuperSoco485;

/**
 The error string is based on this table:

 ## Error Codes

| Byte | Bit | ErrorCode | Report Unit | Report Bit                | Other                                                                                    |
| ---- | --- | --------- | ----------- | ------------------------- | ---------------------------------------------------------------------------------------- |
| 6    | 0   | 99        | Ctrl        | Disconnect                |                                                                                          |
| 6    | 1   | 98        | Ctrl        | Ctrl (6,1 \| 2 \| 4 \| 5) | over current?</br> motor blocking= bit5</br> under voltage = bit4</br> over temperature? |
| 6    | 2   | 97        | Ctrl        | Ctrl (6,0)                |                                                                                          |
| 6    | 3   | 96        | Ctrl        | Ctrl (6,0)                |                                                                                          |
| 6    | 4   | 95        | Ctrl        | Ctrl (6,6)                |                                                                                          |
| 6    | 5   | 94        | Battery     | Disconnect                |                                                                                          |
| 6    | 6   | 93        | Battery     | Battery (8,1)             | charge current to high                                                                   |
| 6    | 7   | 92        | Battery     | Battery (8,0)             | charging stopped                                                                         |
| 7    | 0   | 91        | Battery     | Battery[Temp] >= 3B(60°C) |                                                                                          |
| 7    | 1   | 90        | Battery     | Battery (8,2)             | discharge current too high                                                               |
| 7    | 2   | 89        | Battery     | Battery (8,5)             |                                                                                          |
| 7    | 3   | 88        | Battery     | Battery (8,7)             |                                                                                          |
| 7    | 4   | 87        | X           |                           |                                                                                          |
| 7    | 5   | 86        | X           |                           |                                                                                          |

Ctrl(1,2) means the bit is at Ctrl pdu byte 1, bit 2
[Source](https://github.com/stprograms/SuperSoco485Monitor?tab=readme-ov-file#error-codes)

*/
const char *ErrorCode::toString() const
{
    switch (this->code)
    {
    case 88:
        return "Battery error 7";
    case 89:
        return "Battery error 5";
    case 90:
        return "Discharge current to high";
    case 91:
        return "Battery temperature > 60°C";
    case 92:
        return "Charging stopped";
    case 93:
        return "Charge current to high";
    case 94:
        return "Battery disconnected";
    case 95:
        return "Controller error 6";
    case 96: /* fallthru */
    case 97:
        return "Controller error 0";
    case 98:
        switch (this->detail)
        {
        case 4:
            return "Controller error - undervoltage";
        case 5:
            return "Controller error - motor blocking";
        default:
            return "Controller error special";
        }
    case 99:
        return "Controller disconnected";
    default:
        return "Unknown";
    }
}
