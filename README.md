# SuperSoco485 Arduino Library
[![Run PlatformIO Unit Tests](https://github.com/stprograms/SuperSoco485/actions/workflows/run-unit-tests.yml/badge.svg)](https://github.com/stprograms/SuperSoco485/actions/workflows/run-unit-tests.yml)

Arduino Library for fetching information of Super Soco Motorcycles over RS485.

## Overview
The older Super Soco bikes like the TC series (eg. TC Max) use RS485 to communicate between the different ECUs. It is possible to read the communication using an RS485 interface module and extract the vehicle data from the communication. The protocol has been unofficially decoded partly which is the base for this library. Further information on the protocol itself can be found in the repository of the [SuperSoco485Monitor](https://github.com/stprograms/SuperSoco485Monitor). Additionally to the documentation of the protocol, the application in that repository is a C# console application for monitoring, catching and replaying the traffic on the vehicle.

## Usage
This library was refactored to be hardware independent. To be able to use this library, raw data received from a serial interface (like RS485) needs to be fed to the library. When the [MKR 485 shield](https://docs.arduino.cc/hardware/mkr-485-shield) is used, the library `ArduinoRS485` may be used.

1. Include the `SuperSoco485.h` header, this will provide the `SuperSoco` instance.
2. Call `SuperSoco.begin` and provide and optional callback and user data that will be called if vehicle data has changed.
3. Read raw data from the serial interface and feed the read bytes to the `SuperSoco.parseChunk` function
4. If new data has been parsed, the data in the `SuperSoco` instance is updated and the callback is called in case it was provided with the `begin` function.
5. The current data of the vehicle can be accessed with `SuperSoco._status`

See the examples in the `examples` folder.

## Energy optimization
In case the interfaces are temporarily shut down to save energy, it is possible that incomplete data lies in the library buffer. To reset the buffer and start with a clean one when the interfaces are enabled again, the `flush` function can be used.


## Further information
More information can be found in the examples. Also feel free to dig into the code itself and check out the SuperSoco485Monitor project for further details on the telegrams and the protocol.
