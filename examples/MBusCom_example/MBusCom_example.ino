/*
Example of MBusCom, Arduino M-Bus communication library
 * 
 * License: GPLv3 https://www.gnu.org/licenses/gpl.txt
 */


#define MBUS_ADDRESS 254 // Broadcast
#define START_ADDRESS 0x13 

#include "MBusCom.h"
#include "MBusinoLib.h"
#include "ArduinoJson.h"


// Serial interface used for M-Bus
#include <HardwareSerial.h>

#if defined(ESP32)
HardwareSerial MbusSerial(1);
#else
HardwareSerial MbusSerial;
#endif

MBusCom MBusCom(&MbusSerial,20,21); // The constructor takes the Serial Interface and UART Pins. Change UART Pins depends on your board.

unsigned long timerMbus = 5000;

void setup() {
  MBusCom.begin();
  Serial.begin(115200);
  delay(1500);
  Serial.println(F("startup"));
  Serial.println(F("mbus:"));
  Serial.print(F("  slave address: "));
  Serial.println(MBUS_ADDRESS);
  delay(1000); // let the serial initialize, or we get a bad first frame
}

void loop() {
  if(millis() - timerMbus > 5000){ // Request M-Bus Records
    timerMbus = millis();

    MBusCom.request_data(MBUS_ADDRESS); // <---------------------- MBusCom
  }

  if(millis() - timerMbus > 1500){ // Receive and decode M-Bus Records
    bool mbus_good_frame = false;
    byte mbus_data[MBUS_DATA_SIZE] = { 0 };

    mbus_good_frame = MBusCom.get_response(mbus_data, sizeof(mbus_data)); // <---------------------- MBusCom

    if (mbus_good_frame) {
      int packet_size = mbus_data[1] + 6; 
      MBusinoLib payload(254);  
      JsonDocument jsonBuffer;
      JsonArray root = jsonBuffer.add<JsonArray>();  
      uint8_t fields = payload.decode(&mbus_data[START_ADDRESS], packet_size - START_ADDRESS - 2, root); 
      char jsonstring[4096] = { 0 };
      uint8_t address = mbus_data[5]; 
      serializeJson(root, jsonstring);

      Serial.println("###################### new message ###################### "); 
      Serial.println(String("error = " + String(payload.getError())).c_str()); 
      Serial.println(String("jsonstring = " + String(jsonstring)).c_str());  
      Serial.println(String("SlaveAddress = " + String(address)).c_str());      
      
      for (uint8_t i=0; i<fields; i++) {
        uint8_t code = root[i]["code"].as<int>();
        const char* name = root[i]["name"];
        const char* units = root[i]["units"];           
        double value = root[i]["value_scaled"].as<double>(); 
        const char* valueString = root[i]["value_string"];            

        //values comes as number or as ASCII string or both
        if(valueString != NULL){  // send the value if a ascii value is aviable (variable length)
          Serial.println(String(String(i+1) + "_vs_" + String(name)+ " = " + String(valueString)).c_str()); 
        }

        Serial.println(String(String(i+1) + "_" + String(name) + " = " + String(value,3)).c_str()); //send the value if a real value is aviable (standard)
              
        if(units != NULL){ // send the unit if is aviable
          Serial.println(String(String(i+1) + "_" + String(name) +"_unit = " + String(units)).c_str());
        }
      }
      Serial.println("###################### end of message ###################### ");  
    }
    else{
      Serial.print(F("mbus: bad frame: "));
      //Serial.println(String(mbus_data).c_str());
      //print_bytes(mbus_data, sizeof(mbus_data));
    }       
  }
}
