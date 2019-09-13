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

char unit;
void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet '");
    // read packet
    for (int i = 0; i < packetSize; i++) {
      unit = (char)LoRa.read();
      Serial.print("Reading ")
      Serial.print(unit)
      Serial.println(" from LoRa")
      if (i == 0) {
        switch(unit){
          case 'w':
            //Serial.write(0xFF)
            Serial.println("Handling 'w' case")
            break;
          case 'a':
            break;
          default:
            break;
        }
      } 
      Serial.print("Writing ")
      Serial.print(unit)
      Serial.println(" onto serial")
      //Serial.write(buf)
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
