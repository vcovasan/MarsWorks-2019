#include <SPI.h>
#include <LoRa.h>

uint8_t wheel1pwm = 0;
uint8_t wheel2pwm = 0;
uint8_t wheel3pwm = 0;
uint8_t wheel4pwm = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  Serial.println("LoRa Receiver");
  LoRa.setPins(35, 37, 12);
  LoRa.setSPIFrequency(1E6);
  LoRa.setSpreadingFactor(7);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

char mode;
char dir;
char pwm;
void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
Serial.write('!');
    //Serial.println("Received packet:");
    // read packet
    for (int i = 0; i < packetSize; i++) {
      Serial.println();
      unit = (char)LoRa.read();
      Serial.println();
      //Serial.print("Reading ");
      // Serial.print(unit);
      //Serial.println(" from LoRa");
      if (i == 0) {
        switch (unit) {
          case 'w':
          
            Serial.write(0x1B); 
            Serial.write(0xEE);
            Serial.write(0xF0);
            Serial.write(0x09);
            //Serial.println("Handling 'w' case");
            break;
          case 'a':
            break;
          default:
            break;
        }
      }
      if( i == 1 ){
        dir = unit;
      }
      if( i==2 ){
        switch (dir){
          case 'f': // can't do switch with literal variables 
          
          break; 
          case 'b':
          break;
          case 'l':
          break;
          case 'r':
          break;
        }
      }
//      Serial.print("Writing ");
//      Serial.print(unit);
//      Serial.println(" onto serial");
      Serial.write(unit);
    }
  }
}

void forward(int pwm){
  
  
  }


void wheel_handler(uint8_t pwm, char mode) {
  pwmVal = 27 * pwm;
  if(mode == 'f'){
    leftWheels(pwmVal);
    rightWheels(pwmVal);
    }
   if(mode == 'b'){
    leftWheels(-pwmVal);
    rightWheels(-pwmVal);
    }
    if(mode == 'l'){
      leftWheels(0);
      rightWheels(pwmVal);
      }
    if(mode == 'r'){
    leftWheels(pwmVal);
    rightWheels(0);
    }
  // If fwd - set both pwm values +ve
  // If rv - set both pwm values -ve
  // If left -  
  
}


void sendCANMessage(uint8_t messageType, char* address, char* data) {
    
}
