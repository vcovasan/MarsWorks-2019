/* SensorBoard.ino 
 * Written by Luke Bentley-Fox
 * 
 * This file is flashed to the Arduino Leonardo on the tofSensor board which is
 * connected to the main controller via CAN. 
 * 
 * ASSUMED MSG FORMAT FOR DESTINATION 0x1BEEF019
 * byte1 - lid open state
 *  0 - close
 *  1 - open
 * byte2 - lid lock state
 *  0 ignore
 *  1 unlock
 *  2 lock
 * byte3 - datarequestState  
 *  0 ignore
 *  1 sendData
 **/
#include <SPI.h>
#include <mcp_can.h> //CAN library
#include <Wire.h>    //I2C library
#include <VL6180X.h> //Time Of Flight library
#include <Servo.h>   //Servo library

// Define CAN addresses
#define IDRX  0x1BEEF019;
#define IDTX1 0x1BEEF023;
#define IDTX2 0x1BEEF024;
#define IDTX3 0x1BEEF025;

// Define actuator pins
#define S1 13; // door servo
#define S2 14; // lock servo

// Define sensors pins
#define TS 25; // temp/humi
#define MS 24; // moisture

// Initialise CAN variables
long unsigned int rx_address;
long unsigned int ext;
unsigned char len = 0;
unsigned char buf[8];
const long unsigned int tx_1_address = IDTX1;
const long unsigned int tx_2_address = IDTX2;
const long unsigned int tx_3_address = IDTX3;
unsigned char tx_1_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char tx_2_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char tx_3_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Initialise actuator variables
Servo lidServo;
Servo lockServo;

// Initialise sensor variables
VL6180X tofSensor;
uint16_t depth;
float temperature;
float humidity;
uint16_t moisture;
float weight;

const int SPI_CS_PIN = 15; 
MCP_CAN CAN(SPI_CS_PIN);

/**
 * Package sensor readings into a message and send to the CAN bus
 */
void transmitData() {
  char* int_char_depth = (void*)(&depth);
  char* float_char_temp = (void*)(&temperature);
  tx_1_buf[0] = int_char_depth[0];
  tx_1_buf[1] = int_char_depth[1];
  tx_1_buf[2] = int_char_depth[2];
  tx_1_buf[3] = int_char_depth[3];
  tx_1_buf[4] = float_char_temp[0];
  tx_1_buf[5] = float_char_temp[1];
  tx_1_buf[6] = float_char_temp[2];
  tx_1_buf[7] = float_char_temp[3];
  CAN.sendMsgBuf(tx_1_address, 1, 8, tx_1_buf);
  char* float_char_humidity = (void*)(&humidity);
  char* int_char_moisture = (void*)(&moisture);
  tx_2_buf[0] = float_char_humidity[0];
  tx_2_buf[1] = float_char_humidity[1];
  tx_2_buf[2] = float_char_humidity[2];
  tx_2_buf[3] = float_char_humidity[3];
  tx_2_buf[4] = int_char_moisture[0];
  tx_2_buf[5] = int_char_moisture[1];
  tx_2_buf[6] = int_char_moisture[2];
  tx_2_buf[7] = int_char_moisture[3];
  CAN.sendMsgBuf(tx_2_address, 1, 8, tx_2_buf);
  char* float_char_weight = (void*)(&weight);
  tx_3_buf[0] = float_char_humidity[0];
  tx_3_buf[1] = float_char_humidity[1];
  tx_3_buf[2] = float_char_humidity[2];
  tx_3_buf[3] = float_char_humidity[3];
  CAN.sendMsgBuf(tx_3_address, 1, 4, tx_3_buf);
}

/**
 * Read and handle messages from the can bus relevant to this controller
 */
void receiveCAN() {
  CAN.readMsgBuf(&rx_address,&ext, &len, buf);
  switch (rx_address) {
    case 0x1BEEF001: //emergency stop destination
      break;
    case 0x1BEEF002: //reset destination
      break;
    case IDRX: // us!
      char desiredOpenState = buf[0];
      switch (desiredOpenState) {
        case 0: // unlock lid
          lidServo.writeMicroseconds(1000); // CALIBRATE
        break;
        case 1: // lock lid
          lidServo.writeMicroseconds(2000); // CALIBRATE
        break;
        default:
        break;
      }
      char desiredLockState = buf[1];
      switch (desiredLockState) {
        case 1: // unlock the lid
          lockServo.writeMicroseconds(2000); // CALIBRATE
        break;
        case 2: // lock the lid
        lockServo.writeMicroseconds(1000); // CALIBRATE
        break;
        default:
        break;        
      }
      char requestingData = buf[2];
      if (requestingData == '1'){
        transmitData();
      }
      break;
    default:
      break;
  }
}

void setup() {
  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, LOW);
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
    delay(200);
  }
  Wire.begin();
  tofSensor.init();
  tofSensor.configureDefault();
  tofSensor.setTimeout(500);
  lidServo.attach(S1);
  lockServo.attach(S2);
  lidServo.writeMicroseconds(1500); // CALIBRATE to halfway
  lockServo.writeMicroseconds(1500); // CALIBRATE to halfway
}

void loop() {
  depth = tofSensor.readRangeSingleMillimeters() - 100; //CALIBRATE
  temperature = analogRead(TS);
  humidity = 0; //read in from ts?
  moisture = analogRead(MS);
  weight = 0;
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    receiveCAN();
    delay(1000);
  }
}
