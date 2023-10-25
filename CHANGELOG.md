# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.2.1] - 2023-10-25
### Fixed
 - Blocking of application in case data is available [#7]

## Added
 - Examples for usage of the library
 - Documentation in README.md

## [0.2.0] - 2023-06-09
### Fixed
- Changed type of BMS temperature to int8 [#3]
- Changed type of BMS current to int8 [#3]
- changed type of ECU temperature to int8

### Modified
- Changed API to be more Arduino like (global SuperSoco variable)
- Replaced callback registration with global functions
- Renamed BMS method isCharging to getActivity [#3]

### Added
- Extraction of BMS VBreaker value [#3]
- Implemented standby functionallity [#5]

## [0.1.0] - 2023-03-05

### Added
- Implemented TelegramParser
- Implemented BatteryStatus
- Implemented SuperSoco485 base class
- Implemented ECU Status
