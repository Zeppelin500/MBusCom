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

#include "MBusCom.h"

// ----------------------------------------------------------------------------

MBusCom::MBusCom(HardwareSerial *MbusSerial, uint8_t rxPin, uint8_t txPin){
    _rxPin = rxPin;
    _txPin = txPin;
    _MbusSerial = MbusSerial;
}
/*MBusCom::MBusCom(SerialUART *MbusSerial){
    _rxPin = 0;
    _txPin = 0;
    _MbusSerial = MbusSerial;
}*/
MBusCom::MBusCom(HardwareSerial *MbusSerial){
    _rxPin = 0;
    _txPin = 0;
    _MbusSerial = MbusSerial;
}

MBusCom::~MBusCom(void){}

void MBusCom::begin(){
// ESP32 Syntax, for other platforms has to be extend with #ifdefinied(ESP32)....
  #if defined(ESP8266)
  _MbusSerial->setRxBufferSize(256);
  _MbusSerial->begin(MBUS_BAUD_RATE, SERIAL_8E1);
  #elif defined(ESP32)
  _MbusSerial->setRxBufferSize(256);
  _MbusSerial->begin(MBUS_BAUD_RATE, SERIAL_8E1, _rxPin, _txPin);
  #else
  //_MbusSerial->setRxBufferSize(256);
  _MbusSerial->begin(MBUS_BAUD_RATE, SERIAL_8E1);
  #endif
}

void MBusCom::short_frame(byte address, byte C_field) {
  byte data[6];

  data[0] = 0x10;
  data[1] = C_field;
  data[2] = address;
  data[3] = data[1] + data[2];
  data[4] = 0x16;
  data[5] = '\0';
  _MbusSerial->write((char *)data,5);
}

void MBusCom::request_data(byte address) {
  short_frame(address, 0x5b);
}

bool MBusCom::get_response(byte *pdata, unsigned char len_pdata) {
  byte bid = 0;             // current byte of response frame
  byte bid_end = 255;       // last byte of frame calculated from length byte sent
  byte bid_checksum = 255;  // checksum byte of frame (next to last)
  byte len = 0;
  byte checksum = 0;
  bool long_frame_found = false;
  bool complete_frame = false;
  bool frame_error = false;
  uint16_t j = 0;

  while (!frame_error && !complete_frame){
    j++;
    if(j>255){
      frame_error = true;
    }
  while (_MbusSerial->available()) {
      byte received_byte = (byte)_MbusSerial->read();
      // Try to skip noise
      if (bid == 0 && received_byte != 0xE5 && received_byte != 0x68) {
        continue;
      }

      if (bid > len_pdata) {
        return MBUS_BAD_FRAME;
      }
      pdata[bid] = received_byte;

      // Single Character (ACK)
      if (bid == 0 && received_byte == 0xE5) {
        return MBUS_GOOD_FRAME;
      }

      // Long frame start
      if (bid == 0 && received_byte == 0x68) {
        long_frame_found = true;
      }

      if (long_frame_found) {
        // 2nd byte is the frame length
        if (bid == 1) {
          len = received_byte;
          bid_end = len + 4 + 2 - 1;
          bid_checksum = bid_end - 1;
        }

        if (bid == 2 && received_byte != len) {  // 3rd byte is also length, check that its the same as 2nd byte
          frame_error = true;
        }
        if (bid == 3 && received_byte != 0x68) {
          ;  // 4th byte is the start byte again
          frame_error = true;
        }
        if (bid > 3 && bid < bid_checksum) checksum += received_byte;  // Increment checksum during data portion of frame

        if (bid == bid_checksum && received_byte != checksum) {  // Validate checksum
          frame_error = true;
        }
        if (bid == bid_end && received_byte == 0x16) {  // Parse frame if still valid
          complete_frame = true;
        }
      }
      bid++;
      yield();
    }
  }

  if (complete_frame && !frame_error) {
    return MBUS_GOOD_FRAME;
  } else {
    return MBUS_BAD_FRAME;
  }
}

void MBusCom::normalize(byte address) {
  short_frame(address,0x40);
}

void MBusCom::clearRXbuffer(){

  while (_MbusSerial->available()) {
      byte received_byte = (byte)_MbusSerial->read();
  }
}

void MBusCom::set_address(byte oldaddress, byte newaddress) {
 
  byte data[13];
 
  data[0] = MBUS_FRAME_LONG_START;
  data[1] = 0x06;
  data[2] = 0x06;
  data[3] = MBUS_FRAME_LONG_START;
  
  data[4] = 0x53;
  data[5] = oldaddress;
  data[6] = 0x51;
  
  data[7] = 0x01;         // DIF [EXT0, LSB0, FN:00, DATA 1 8BIT INT]
  data[8] = 0x7A;         // VIF 0111 1010 bus address
  data[9] = newaddress;   // DATA new address
  
  data[10] = data[4]+data[5]+data[6]+data[7]+data[8]+data[9];
  data[11] = 0x16;
  data[12] = '\0';
  
  _MbusSerial->write((char*)data,12);
}



