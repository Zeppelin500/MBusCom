# MBusinoCom - an Arduino M-Bus data link layer communication library. 
[![version](https://img.shields.io/badge/version-0.1.0-brightgreen.svg)](CHANGELOG.md)
[![license](https://img.shields.io/badge/license-GPL--3.0-orange.svg)](LICENSE)


## Documentation

The **MBusCom** library enables Arduino devices to communicate with M-Bus Slaves thru a M-Bus Master (level converter) at the Data Link Layer

Tested at ESPs, additional platforms will be supported in the future.

Together with the [**MBusinoLib**](https://github.com/Zeppelin500/MBusinoLib), you can build a M-Bus Master for reading out M-Bus Slaves.

A working M-Bus --> MQTT gateway with this library [**MBusino**](https://github.com/Zeppelin500/MBusino)

### Credits

Thanks to **HWHardsoft** and **TrystanLea** for parts of the M-Bus communication: https://github.com/HWHardsoft/emonMbus and https://github.com/openenergymonitor/HeatpumpMonitor

## API

### Class: MBusCom

Include and instantiate the MBusCom class. The constructor takes the Serial Interface and UART Pins.

```MBusCom(HardwareSerial *MbusSerial, uint8_t rxPin, uint8_t txPin);```

### begin
```MBusCom.begin();```

start the Serial communication

### normalize
```MBusCom.normalize(byte address);```

set the bus back

### set_address
```MBusCom.set_address(byte oldaddress, byte newaddress);```

change the M-Bus address of a slave. Use 254 as oldaddress to change a single slave with unknown address.

### request_data
```MBusCom.request_data(byte address)```

Request RSP_UD telegrams. (Slave data records)

### get_response
```MBusCom.get_response(byte *pdata, unsigned char len_pdata);```

Get the slave response from the rx buffer.

### clearRXbuffer
```MBusCom.clearRXbuffer();```

Clears the rx buffer, if some junk received



## References

* [The M-Bus: A Documentation Rev. 4.8 - Appendix](https://m-bus.com/assets/downloads/MBDOC48.PDF)
* [Dedicated Application Layer (M-Bus)](https://datasheet.datasheetarchive.com/originals/crawler/m-bus.com/ba82a2f0a320ffda901a2d9814f48c24.pdf) by H. Ziegler

## License


The MBusCom library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The MBusCom library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the MBusinoLib library.  If not, see <http://www.gnu.org/licenses/>.
