#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
  // configure LoRa
  LoRa.setPins(35, 37, 10);
  LoRa.setSPIFrequency(1E6);
  LoRa.setSpreadingFactor(7);
  delay(1000);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  delay(2000);
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    switch((char)LoRa.read()) {
      case 'w':
        forwardWheelMessages();
        break;
      case 'a':
        forwardArmMessages();
        break;
      case 'c':
        forwardCameraMessage();
      case 'b':
        forwardBoxMessages();
      default:
        break;
    }
  } 
}

long unsigned int address;
char direction;
int mag = 0;
int pwm = 0;
void forwardWheelMessages(){
  //left motors
  address = 0x1BEEF005;
  direction = (char)LoRa.read();
  mag = (int)LoRa.read();
  switch(direction){
    case 'f':
      pwm=(mag * 27;      
      break;
    case 'b':
      pwm=(mag * -27;      
      break;
    default:
      break;
  }
  char* address_ptr = (void*)(&address);
  char* pwm_ptr = (void*)(&pwm);
  Serial1.write('!');
  Serial1.write(address_ptr[0]);
  Serial1.write(address_ptr[1]);
  Serial1.write(address_ptr[2]);
  Serial1.write(address_ptr[3]);
  Serial1.write(pwm_ptr[0]);
  Serial1.write(pwm_ptr[1]);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(pwm_ptr[0]);
  Serial1.write(pwm_ptr[1]);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial3.write('!');
  Serial3.write(address_ptr[0]);
  Serial3.write(address_ptr[1]);
  Serial3.write(address_ptr[2]);
  Serial3.write(address_ptr[3]);
  Serial3.write(pwm_ptr[0]);
  Serial3.write(pwm_ptr[1]);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(pwm_ptr[0]);
  Serial3.write(pwm_ptr[1]);
  Serial3.write(0x00);
  Serial3.write(0x00);
  //right motors
  address = 0x1BEEF009;
  direction = (char)LoRa.read();
  mag = (int)LoRa.read();
  switch(direction){
    case 'f':
      pwm=(mag * 27;      
      break;
    case 'b':
      pwm=(mag * -27;      
      break;
    default:
      break;
  }
  address_ptr = (void*)(&address);
  pwm_ptr = (void*)(&pwm);
  Serial1.write('!');
  Serial1.write(address_ptr[0]);
  Serial1.write(address_ptr[1]);
  Serial1.write(address_ptr[2]);
  Serial1.write(address_ptr[3]);
  Serial1.write(pwm_ptr[0]);
  Serial1.write(pwm_ptr[1]);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(pwm_ptr[0]);
  Serial1.write(pwm_ptr[1]);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial3.write('!');
  Serial3.write(address_ptr[0]);
  Serial3.write(address_ptr[1]);
  Serial3.write(address_ptr[2]);
  Serial3.write(address_ptr[3]);
  Serial3.write(pwm_ptr[0]);
  Serial3.write(pwm_ptr[1]);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(pwm_ptr[0]);
  Serial3.write(pwm_ptr[1]);
  Serial3.write(0x00);
  Serial3.write(0x00);
}  

void forwardArmMessages(){
  //Serial.write(0x12);
  Serial.write(0x13);
}

void forwardCameraMessages(){}

void forwardCollectionMessages(){}
