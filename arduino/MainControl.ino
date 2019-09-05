#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*
 * Written By: Tendai Makumire
 * Last Updated: 1-Sept-2019
 * Description: This code sends a 'GET' message to another Arduino using a RF24 (2.4GHz Radio Transceiver) and waits for the data
 * to be sent back to it before displaying it on serial moniter
 */

#define GET_TEMP 1
#define GET_MOIST 2
#define GET_WEIGHT 3
#define GET_DATA 4

RF24 radio(7, 8); // CE, CSN
uint8_t addresses[][6] = {"1Node","2Node"};

struct MyData {
  uint8_t x_pos;
  uint8_t y_pos;
  uint8_t z_pos;
  uint8_t packetID;
  
};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 1Node
  radio.openReadingPipe(1, addresses[1]); // 2Node 
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {

  radio.startListening();
  if(radio.available()) {

    
    Serial.println("Request received");
    uint8_t messageCode = 0;
    radio.read(&messageCode,sizeof(messageCode)); 
    Serial.println(messageCode);


    if(messageCode == GET_TEMP) {
      Serial.println("Get the temperature");
      radio.stopListening();
      uint8_t temperature = 24;
      Serial.println(radio.write(&temperature,sizeof(temperature)));
      Serial.println("Sensor Reading Sent");
      delay(10);
      radio.startListening();
       
     }

    if(messageCode == GET_MOIST) {
      Serial.println("Get the moisture level");
      radio.stopListening();
      uint8_t moisture = 700;
      Serial.println(radio.write(&moisture,sizeof(moisture)));
      
      delay(10);
      radio.startListening();
       
     }

    if(messageCode == GET_WEIGHT) {
      Serial.println("Get the weight");
      radio.stopListening();
      uint8_t weight = 100;
      Serial.println(radio.write(&weight,sizeof(weight)));
      Serial.println("Sensor Reading Sent");
      delay(10);
      radio.startListening();
       
     }
     
    if(messageCode == GET_DATA) {
      // Setup Struct Data
      MyData myData;
      myData.x_pos = 10;
      myData.y_pos = 15;
      myData.z_pos = 20;
          
      Serial.println("Package the position data");
      radio.stopListening();
      Serial.println("Position Data Transmit Result:");
      Serial.println(radio.write(&myData,sizeof(myData)));
      
      delay(10);
      radio.startListening();
       
     }
             
   }
    
}

