# MBusCom change log

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [0.1.7] - 2026-05-16

### Fixed

- RX buffer too small for max-length M-Bus telegrams (256 → 271 bytes)
- Byte overflow in `get_response()` — `bid`, `bid_end`, `bid_checksum` changed from `byte` to `uint16_t`
- Timeout check too short for long frames (255 → 261)
- Added `MBUS_MAX_TELEGRAM_LEN` define (261 bytes per EN 13757-2)

## [0.1.6] - 2025-06-30

### added

- aplication reset
- read rx buffer for debug

## [0.1.5] - 2025-05-26

### added

- request with FCB
- MBusCom::available()

## [0.1.4] - 2025-03-28

### Changed

- better syntax in example

## [0.1.3] - 2025-02-27

### Changed

- Typo in library.properties

## [0.1.2] - 2025-02-27

### Changed

- improvements with the constructor

## [0.1.1] - 2025-02-26

### Changed

- typos

## [0.1.0] - 2025-02-18
- Initial version
