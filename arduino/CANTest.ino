#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>

long unsigned int rxId;   //CAN definitions
unsigned long rcvTime;
unsigned char len = 0;
unsigned char buf[8];
long unsigned int txID = 0xDEADBEEF;
unsigned char stmp[8] = {0x0E, 0x00, 0xFF, 0x22, 0xE9, 0xFA, 0xDD, 0x51};
const int SPI_CS_PIN = 5;

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);
//  pinMode(2, OUTPUT);
//  pinMode(0, OUTPUT);
//  digitalWrite(2, HIGH);
//  digitalWrite(0, LOW);
  while (CAN_OK != CAN.begin(CAN_250KBPS))              // init can bus : baudrate = 500k
  {
     Serial.println("CAN BUS Module Failed to Initialized");
    Serial.println("Retrying....");
    delay(200);
  }
//  digitalWrite(2, LOW);
  Serial.println("CAN BUS Module Initialized!");
  Serial.println("Time\t\tPGN\t\tByte0\tByte1\tByte2\tByte3\tByte4\tByte5\tByte6\tByte7");
}

void loop() {
  
 // digitalWrite(2, HIGH);
  if(Serial.available() > 2){
    int sendingflot = Serial.parseInt();
    Serial.flush();
    char sendbuf[8] = "\0\0\0\0\0\0\0";
   // Serial.write();
    CAN.sendMsgBuf(0xDDDDDDDE, 1, sizeof(sendingflot), (void*)(&sendingflot));
  }
  else
  Serial.flush();
  
  if (CAN_MSGAVAIL == CAN.checkReceive())           // check if data coming
  {
    rcvTime = millis();
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
   // if(len == sizeof(buf))
    {
      rxId = CAN.getCanId();
      
      Serial.print(rcvTime);
      Serial.print("\t\t");
      Serial.print("0x");
      Serial.print(rxId, HEX);
      Serial.print("\t");
      
      if(rxId == 0x1BEEF003){
        Serial.print(*((long*)(void*)buf));
      } else{
        Serial.write(buf, sizeof(buf));
      }
      buf[8] = "\0\0\0\0\0\0\0";
      Serial.print('\n');
    }
  }
 // digitalWrite(2, LOW);
  delay(10);

}
