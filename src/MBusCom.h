/*

MBusCom, Arduino M-Bus communication library


The MBusComlibrary is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The MBusCom library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the MBusinoLib library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef MbusCom_H
#define MbusCom_H

#include <Arduino.h>

#define MBUS_BAUD_RATE 2400
#define MBUS_ADDRESS 0xFE  // brodcast
#define MBUS_TIMEOUT 1000  // milliseconds
#define MBUS_DATA_SIZE 255
#define MBUS_GOOD_FRAME true
#define MBUS_BAD_FRAME false

#define MBUS_FRAME_SHORT_START          0x10
#define MBUS_FRAME_LONG_START           0x68
#define MBUS_FRAME_STOP                 0x16

#define MBUS_CONTROL_MASK_SND_NKE       0x40
#define MBUS_CONTROL_MASK_DIR_M2S       0x40
#define MBUS_ADDRESS_NETWORK_LAYER      0xFE

//HardwareSerial MbusSerial(1);

class MBusCom {

public:
  
  MBusCom(HardwareSerial *MbusSerial ,uint8_t rxPin, uint8_t txPin);
  ~MBusCom();

  void begin();

  void clearRXbuffer();
  void set_address(byte oldaddress, byte newaddress);
  void normalize(byte address);
  void request_data(byte address);
  bool get_response(byte *pdata, unsigned char len_pdata);
  
protected:

  uint8_t _hardwareSerialNr;
  uint8_t _rxPin;
  uint8_t _txPin;

  HardwareSerial *_MbusSerial;

  void short_frame(byte address, byte C_field);

};
#endif
