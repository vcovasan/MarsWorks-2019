#include <SPI.h>
#include <mcp_can.h> //CAN library

// Initialise CAN variables
long unsigned int rx_address;
uint8_t ext;
unsigned char len = 0;
unsigned char buf[8];
unsigned char tx_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const int SPI_CS_PIN = 11;
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
  Serial.println("CAN Initialised");
}

void sendToSensor() {
  tx_buf[0] = '0';
  tx_buf[1] = '0';
  tx_buf[2] = '1';
  byte sndStat = CAN.sendMsgBuf(0x1BEEF019, 1, 3, tx_buf);
  if (sndStat == CAN_OK) {
    Serial.print("Successfully sent message to address");
  } else {
    Serial.print("Failed to send message to address");
  }
  Serial.println(0x1BEEF019, HEX);
}

void sendToMotor() {
  byte pwm[8] = {0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00};
  byte sndStat = CAN.sendMsgBuf(0x1BEEF009, 1, 3, pwm);
  if (sndStat == CAN_OK) {
    Serial.print("Successfully sent message to address ");
  } else {
    Serial.print("Failed to send message to address ");
  }
  Serial.println(0x1BEEF009, HEX);
}

void receiveCAN() {
  CAN.readMsgBuf(&rx_address, &ext, &len, buf);
  Serial.print("Noticed a message destined for address: ");
  Serial.println(rx_address, HEX);
  switch (rx_address) {
    case 0x1BEEF001: //emergency stop destination
      break;
    case 0x1BEEF002: //reset destination
      break;
    case 0x1BEEF023: //d&t
      Serial.print("Handling message sent to ");
      Serial.println(rx_address, HEX);
      break;
    case 0x1BEEF024: //h&m
      Serial.print("Handling message sent to ");
      Serial.println(rx_address, HEX);
      break;
    case 0x1BEEF025: //w
      Serial.print("Handling message sent to ");
      Serial.println(rx_address, HEX);
      break;
    case 0x1BEEF007: //from motor
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
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    receiveCAN();
    delay(100);
  }
  sendToSensor();
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    receiveCAN();
    delay(100);
  }
  sendToMotor();
}
