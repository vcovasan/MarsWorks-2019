#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
  // configure LoRa
  LoRa.setPins(35, 37, 12);
  LoRa.setSPIFrequency(1E6);
  LoRa.setSpreadingFactor(7);
  delay(1000);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

int js = 5;
int pwm = 0;
void loop() {
  delay(2000);
  pwm = js * 27;
  static_cast<char*>(static_cast<void*>(&pwm));

  Serial.write('!');
  Serial.write(0x1B);
  Serial.write(0xEE);
  Serial.write(0xF0);
  Serial.write(0x09);
  Serial.write(pwm, 2)
  Serial.write(0x00)
  Serial.write(0x00)
  Serial.write(pwm, 2)
  Serial.write(0x00)
  Serial.write(0x00)
}
  
  // int packetSize = LoRa.parsePacket();
  // if (packetSize) {
  //   // read packet
  //   for (int i = 0; i < packetSize; i++) {
  //     unit = (char)LoRa.read();
  //     Serial.print("Reading ")
  //     Serial.print(unit)
  //     Serial.println(" from LoRa")
  //     if (i == 0) {
  //       switch(unit){
  //         case 'w':
  //           Serial.write(0xFF)
  //           break;
  //         case 'a':
  //           break;
  //         default:
  //           break;
  //       }
  //     } 
  //     Serial.write(buf)
  //   }
  // }
}
