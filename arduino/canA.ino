/*
 * Things I have learnt.
 * 
 * We MUST use normal mode ALWAYS. this is set either by us or our libraries
 * in the CANCTRL register at address XFh - check this to be sure we are not on loopback.
 * 
 * We can modify the value of EFLAG maybe to stop the node from blocking itself

 */
#include <mcp_can.h>  //coryjfowler/MCP_CAN_lib
#include <mcp_can_dfs.h> //coryjfowler/MCP_CAN_lib
#include <SPI.h>
#define EREG 0x2D
#define IREG 0x2C

// Initialise CAN variables
long unsigned int rx_address;
uint8_t ext;
unsigned char len = 0;
unsigned char buf[8];
unsigned char tx_buf[8] = { '1', 0, 0, 0, 0, 0, 0, 0 };

const int SPI_CS_PIN = 11;
const int SPI_INT_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  // Initialise serial
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Serial Initialised");
  //digitalWrite(SPI_CS_PIN, LOW);
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ)) {
    Serial.println("CAN Initialising...");
    delay(200);
  }
  CAN.setMode(MCP_NORMAL);
  Serial.println("CAN Initialised");
}

void sendToB() {
  tx_buf[0] = '0';
  tx_buf[1] = '0';
  tx_buf[2] = '1';
  byte sndStat = CAN.sendMsgBuf(0x1BEEF00B, 8, tx_buf);
  if (sndStat == CAN_OK) {
    Serial.print("Successfully sent message to address: ");
  } else {
    Serial.print("Failed to send message to address: ");
  }
  Serial.println(0x1BEEF00B, HEX);
}

void receiveCAN() {
  CAN.readMsgBuf(&rx_address, &len, buf);
  Serial.print("Noticed a message destined for address: ");
  Serial.println(rx_address, HEX);
  switch (rx_address) {
    case 0xA: //us!
      Serial.print("Handling message sent to ");
      Serial.println(rx_address, HEX);
      break;
    default:
      break;
  }
  Serial.print("Message content: ");
  Serial.write(buf, len);
  Serial.println();
}
uint8_t eReg;
uint8_t iReg;
void loop() {
  if (CAN.checkError() == CAN_CTRLERROR) {
    Serial.print("CAN error bit is set.");
    Serial.println("Error Flag Value: " );
    Serial.print(CAN.getError());
    Serial.print(", rxErrorCount: " );
    Serial.print(CAN.errorCountRX());
    Serial.print(", txErrorCount: " );
    Serial.println(CAN.errorCountTX());
 /*   eReg = CAN.mcp2515_readRegister(EREG);              //PRIVATE IN LIB - MAKE PUBLIC
    Serial.println("Error Flag Register Contents...");
    Serial.print("Receive Buffer 1 Overflow Flag: ");
    Serial.println(eReg[0]);
    Serial.print("Receive Buffer 0 Overflow Flag: ");
    Serial.println(eReg[1]);
    Serial.print("Bus-Off Error Flag:"); // TEC = 255 -> comms blackout
    Serial.println(eReg[2]);
    Serial.print("Transmit Error-Passive Flag: "); // TEC > 127 -> only allow passive error frames
    Serial.println(eReg[3]);
    Serial.print("Receive Error-Passive Flag: "); //REC > 127 -> only allow passive error frames
    Serial.println(eReg[4]);
    Serial.print("Transmit Error-Warning Flag: "); // TEC >= 96
    Serial.println(eReg[5]);
    Serial.print("Receive Error-Warning Flag: "); // REC >= 96
    Serial.println(eReg[6]);
    Serial.print("Error Warning Flag: "); // TEC || REC >= 96
    Serial.println(eReg[7]);
    Serial.println();
    iReg = CAN.mcp2515_readRegister(IREG);               //PRIVATE IN LIB - MAKE PUBLIC
    Serial.println("CAN Interrupt Enable Register Contents...");
    Serial.print("Message Error Interrupt Enable Flag: ");//Interrupt on error during message reception or transmission
    Serial.println(iReg[0]);
    Serial.print("Wake-up Interrupt Enable Flag: ");//Interrupt on CAN bus activity
    Serial.println(iReg[1]);
    Serial.print("Error Interrupt Enable Flag: ");//Interrupt on EFLG error condition change
    Serial.println(iReg[2]);
    Serial.print("Transmit Buffer 2 Empty Interrupt Enable Flag: ");//Interrupt on TXB2 becoming empty
    Serial.println(iReg[3]);
    Serial.print("Transmit Buffer 1 Empty Interrupt Enable Flag: ");//Interrupt on TXB1 becoming empty
    Serial.println(iReg[4]);
    Serial.print("Transmit Buffer 0 Empty Interrupt Enable Flag: ");//Interrupt on TXB0 becoming empty
    Serial.println(iReg[5]);
    Serial.print("Transmit Receive Buffer 1 Full Interrupt Enable Flag: ");//Interrupt when message was received in RXB1
    Serial.println(iReg[6]);
    Serial.print("Receive Buffer 0 Full Interrupt Enable: ");//Interrupt when message was received in RXB0
    Serial.println(iReg[7]);
    Serial.println();*/
  }
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    Serial.println("received data");
    receiveCAN();
  }
  sendToB();
  delay(2000);
}
