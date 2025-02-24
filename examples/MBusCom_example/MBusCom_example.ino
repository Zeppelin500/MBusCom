/*
Example of MBusCom with MBusinoLib decoding, Arduino M-Bus communication library
 * 
 * License: GPLv3 https://www.gnu.org/licenses/gpl.txt
 */


#define SLAVE_MBUS_ADDRESS 254 // Broadcast
#define START_ADDRESS 0x13 

#include "MBusCom.h"
#include "MBusinoLib.h"
#include "ArduinoJson.h"

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

    if (mbus_good_frame) {
      int packet_size = mbus_data[1] + 6; 
      MBusinoLib payload(254);  
      JsonDocument jsonBuffer;
      JsonArray root = jsonBuffer.add<JsonArray>();  
      uint8_t fields = payload.decode(&mbus_data[START_ADDRESS], packet_size - START_ADDRESS - 2, root);       
      uint8_t address = mbus_data[5]; 

      Serial.println("###################### new message ###################### "); 
      Serial.println(String("error = " + String(payload.getError())).c_str());  
      Serial.println(String("SlaveAddress = " + String(address)).c_str());  

      /*  // only for uC with a lot of RAM
      char jsonstring[4096] = { 0 };
      serializeJson(root, jsonstring);
      Serial.println(String("jsonstring = " + String(jsonstring)).c_str()); 
      */
      
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
          Serial.println(String(String(i+1) + "_" + String(name) + "_unit = " + String(units)).c_str());
        }
      }
      Serial.println("###################### end of message ###################### ");  
    }
    else{
      Serial.println("mbus: bad frame: ");
      Serial.println("+++++++++");
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
}
