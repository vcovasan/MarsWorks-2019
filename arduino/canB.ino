#include <mcp_can.h>
#include <mcp_can_dfs.h>

#include <SPI.h>

// Initialise CAN variables
long unsigned int rx_address;
uint8_t ext;
unsigned char len = 0;
unsigned char buf[8];
unsigned char tx_buf[8] = { '1',0,0,0,0,0,0,0 };

const int SPI_CS_PIN = 11;
const int SPI_INT_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  // Initialise serial
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Serial Initialised");
  // Initialise CAN
  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, LOW);
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ)) {
    Serial.println("CAN Initialising...");
    delay(200);
  }
  CAN.setMode(MCP_NORMAL);
  Serial.println("CAN Initialised");
}

void sendToA() {
  tx_buf[0] = '0';
  tx_buf[1] = '0';
  tx_buf[2] = '1';
  byte sndStat = CAN.sendMsgBuf(0x1BEEF00A, 3, tx_buf);
  if (sndStat == CAN_OK) {
    Serial.print("Successfully sent message to address: ");
  } else {
    Serial.print("Failed to send message to address: ");
  }
  Serial.println(0x1BEEF00A, HEX);
}

void receiveCAN() {
  CAN.readMsgBuf(&rx_address, &len, buf);
  Serial.print("Noticed a message destined for address: ");
  Serial.println(rx_address, HEX);
  switch (rx_address) {
    case 0xB: //us!
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

void loop() {
  // error-active mode after any error in rx or tx
  // error-passive after >127 in either
  // bus-off  mode after >256 in either
  // resets after a while
  if (CAN.checkError() == CAN_CTRLERROR){
    Serial.print("CAN error bit is set.");
    Serial.println("Error register: " );
    Serial.print(CAN.getError());
    Serial.print(", rxErrorCount: " );
    Serial.print(CAN.errorCountRX());
    Serial.print(", txErrorCount: " );
    Serial.println(CAN.errorCountTX());
    //is possible to read from register 6-2 to see which errors are firing
    CAN.mcp2515_readRegisterS(const INT8U address, INT8U values[], const INT8U n)
  }
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    receiveCAN();
  }
  sendToA();
  delay(2000);
}
