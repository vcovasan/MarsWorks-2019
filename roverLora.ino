#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial1.begin(115200);
  Serial3.begin(115200);
  while (!Serial);
  //Serial.println("LoRa Receiver");
  // configure LoRa
  LoRa.setPins(35, 37, 10);
  LoRa.setSPIFrequency(1E6);
  LoRa.setSpreadingFactor(7);
  delay(1000);

  if (!LoRa.begin(433E6)) {
    //Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
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

//who needs abstractions anyway
long unsigned int motors_tx_address;
char direction;
int mag = 0;
int pwm = 0;
void forwardWheelMessages(){
  //left motors
  address = 0x1BEEF005;
  direction = (char)LoRa.read();
  mag = (int)LoRa.read();
  if (direction == 'f' ){
      pwm=(mag * 27);      
  }
  else if (direction =='b'){
      pwm=(mag * -27);      
  }
  char* address_ptr = (void*)(&motors_tx_address);
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
  motors_tx_address = 0x1BEEF009;
  direction = (char)LoRa.read();
  mag = (int)LoRa.read();
  if (direction == 'f' ){
      pwm=(mag * 27);
  }
  else if (direction =='b'){
      pwm=(mag * -27);      
  }
  address_ptr = (void*)(&motors_tx_address);
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

long unsigned int arm_tx_address;
void forwardArmMessages(){
  //Serial.write(0x12);
  //Serial.write(0x13);
}

void forwardCameraMessages(){}

void forwardCollectionMessages(){}
