/*
Minimal Example of MBusCom without decoding, Arduino M-Bus communication library

 * 
 * License: GPLv3 https://www.gnu.org/licenses/gpl.txt
 */


#define SLAVE_MBUS_ADDRESS 254 // Broadcast
#define START_ADDRESS 0x13 

#include "MBusCom.h"

#include <HardwareSerial.h> // Serial interface used for M-Bus

#if defined(ESP32)
HardwareSerial MbusSerial(1);
MBusCom MBusCom(&MbusSerial,37,39); // The constructor takes the Serial Interface and UART Pins. Change UART Pins depends on your board.
#else
MBusCom MBusCom(&Serial1); // The constructor takes the Serial Interface
#endif

unsigned long timerMbus = 5000;
uint8_t mbusLoopStatus = 0;

void setup() {
  MBusCom.begin();  //<---------------------- MBusCom: set up the Serial interface fur M-Bus communication
  Serial.begin(9600);
  delay(1500);
  Serial.println("startup");
  Serial.println("MBusCom_Example");
  delay(2000); // let the serial initialize, or we get a bad first frame
}

void loop() {
  if(millis() - timerMbus > 5000){ // Request M-Bus Records
    timerMbus = millis();
    mbusLoopStatus = 1;
    Serial.print("Request Address: ");
    Serial.print(SLAVE_MBUS_ADDRESS);
    Serial.println();
    MBusCom.request_data(SLAVE_MBUS_ADDRESS); // <---------------------- MBusCom: request a record telegram from the slave
  }

  if(millis() - timerMbus > 1500 && mbusLoopStatus == 1){ // Receive and decode M-Bus Records
    mbusLoopStatus = 0;
    bool mbus_good_frame = false;
    byte mbus_data[MBUS_DATA_SIZE] = { 0 };
    mbus_good_frame = MBusCom.get_response(mbus_data, sizeof(mbus_data)); // <---------------------- MBusCom: get the reveived telegram from the rx buffer
    Serial.println("received M-Bus Telegram: ");
    
    for(uint8_t i = 0; i<sizeof(mbus_data); i++){  // print the received telegram
      char buffer[3];                                                                            
      sprintf(buffer,"%02X",mbus_data[i]);
      Serial.print(buffer);
      Serial.print(" ");  
    }
    Serial.println();
    Serial.println("+++++++++");
     
  }
}
