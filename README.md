# SuperSoco485 Arduino Library
Arduino Library for fetching information of Super Soco Motorcycles over RS485.

## Overview
The older Super Soco bikes like the TC series (eg. TC Max) use RS485 to communicate between the different ECUs. It is possible to read the communication using an RS485 interface module and extract the vehicle data from the communication. The protocol has been unofficially decoded partly which is the base for this library. Further information on the protocol it self can be found in the repository of the [SuperSoco485Monitor](https://github.com/stprograms/SuperSoco485Monitor), which is a C# console application for monitoring, catching and replaying the traffic on the vehicle.

## Usage
The library is based on the [MKR 485 shield](https://docs.arduino.cc/hardware/mkr-485-shield) using the library `ArduinoRS485`. But in theory it should work with other modules as well that are compatible with the `Arduino RS485` library.

The library uses a global instance of the `SuperSoco485` class, named `SuperSoco`. After initialization with the `begin()` function, the function `update()` needs to be called periodically. `update()` will fetch the received bytes from the serial interface and pushes it to the internal `TelegramParser` class that is reponsible for parsing the data and extracting the telegrams of the ECUs.

If the received data inside the telegrams has changed and the `_status` member of the `SuperSoco` instance has been updated, the global callback function `superSocoDataUpdated()` is called. This function needs to be implemented in the application. Implement an empty function if this event is not needed.

The data of the vehicle can then be fetched by accessing the `_status` member of the `SuperSoco` instance.

## Energy optimization
If it is necessary to reduce the power consumption of the hardware module, the library can be sent to standby and woken up later. If it is in standby, the receiver driver of the module is disabled, so no power is drawn by it. Function `update()` can still be called if the module is in standby, but no data will be received and therefore no data updates are signalled.

## Further information
More information can be found in the examples. Also feel free to dig into the code itself and check out the SuperSoco485Monitor project for further details on the telegrams and the protocol.

If required, the source files `SuperSoco485.cpp` and `TelegramParser.cpp` hold defines for TRACE and/or DEBUG. Uncomment these lines and recompile to get detailled information printed to the serial.