/* SensorBoard.ino IN PROGRESS
 * Written by Luke Bentley-Fox
 * 
 * This file is flashed to the Arduino Leonardo on the sensor board which is
 * connected to the main controller via CAN. 
 * 
 * ASSUMED MSG FORMAT FOR DESTINATION 0x1BEEF019
 * byte1 - lid open state
 *  0 ignore
 *  1 open
 *  2 close
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

// Define CAN addresses
#define IDRX  0x1BEEF019
#define IDTX1 0x1BEEF023
#define IDTX2 0x1BEEF024
#define IDTX3 0x1BEEF025

// Define actuator pins
#define S1  13 // door servo
#define S2  14 // lock servo

// Define sensors pins
#define TMP A2 // temp sensor

// Instantiate CAN variables
long unsigned int rx_address;
unsigned char len = 0;
unsigned char buf[8];
const long unsigned int tx_1_address = IDTX1;
const long unsigned int tx_2_address = IDTX2;
const long unsigned int tx_3_address = IDTX3;
unsigned char tx_1_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char tx_2_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char tx_3_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Instantiate actuator variables

// Instantiate sensor variables
VL6180X sensor;
uint16_t depth;
float temperature;
float humidity;
float moisture;
float weight;

const int SPI_CS_PIN = 15; //mosi, miso: 15,16
MCP_CAN CAN(SPI_CS_PIN);

int STOP = 17;

//gets the depth from the time of flight sensor
uint16_t getDepth(){
  d = sensor.readRangeSingleMillimeters()
  d - 100;
  return d;
}

//gets the temperature from the environment sensor
float getTemp(value){
  value = analogRead(TMP)
}

// updates value of  humidity from the environment sensor
float getHumidity(){}

// updates value of moisture from its sensor
float getMoisture(){}

//updates the value of the weight from the laod balance
float getWeight(){}


void transmitDepthAndTemperature() {
  //get the data
  getDepth(&depth)
  getTemp(&temperature)
  //package the data
  char* float_char_dpth = (void*)(&depth);
  char* float_char_temp = (void*)(&temperature);
  tx_1_buf[0] = float_char_dpth[0];
  tx_1_buf[1] = float_char_dpth[1];
  tx_1_buf[2] = float_char_dpth[2];
  tx_1_buf[3] = float_char_dpth[3];
  tx_1_buf[4] = float_char_temp[0];
  tx_1_buf[5] = float_char_temp[1];
  tx_1_buf[6] = float_char_temp[2];
  tx_1_buf[7] = float_char_temp[3];
  //send the data
  CAN.sendMsgBuf(tx_1_address, 1, 8, tx_1_buf);
}

void transmitHumidityAndMoisture() {
  //get the data
  getHumidity(&humidity)
  getMoisture(&moisture)
  //package the data
  char* float_char_humidity = (void*)(&humidity);
  char* float_char_moisture = (void*)(&moisture);
  tx_2_buf[0] = float_char_humidity[0];
  tx_2_buf[1] = float_char_humidity[1];
  tx_2_buf[2] = float_char_humidity[2];
  tx_2_buf[3] = float_char_humidity[3];
  tx_2_buf[4] = float_char_moisture[0];
  tx_2_buf[5] = float_char_moisture[1];
  tx_2_buf[6] = float_char_moisture[2];
  tx_2_buf[7] = float_char_moisture[3];
  //send the data
  CAN.sendMsgBuf(tx_2_address, 1, 8, tx_2_buf);
}

void transmitWeight() {
  //get the data
  getHumidity(&weight)
  //package the data
  char* float_char_weight = (void*)(&weight);
  tx_3_buf[0] = float_char_humidity[0];
  tx_3_buf[1] = float_char_humidity[1];
  tx_3_buf[2] = float_char_humidity[2];
  tx_3_buf[3] = float_char_humidity[3];
  //send the data
  CAN.sendMsgBuf(tx_3_address, 1, 4, tx_3_buf);
}

void transmitData() {
  transmitDepthAndTemperature()
  transmitHumidityAndMoisture()
  transmitWeight()
}

void ()

// reads messages off the can bus, and handles any relevant to this controller
void receiver() {
  CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
  rx_address = CAN.getCanId();
  switch (rx_address) {
    case 0x1BEEF001: //emergency stop
      break;
    case 0x1BEEF002: //reset
      break;
    case IDRX:
      char desiredOpenState = buf[0];
      switch (desiredOpenState) {
        case 1:
          //open the door
        break;
        case 2:
          //close the door
        break;
        default:
        break;
      }
      char desiredLockState = buf[1];
      switch (desiredLockState) {
        case 1:
          //unlock the lid
        break;
        case 2:
          //close the lid
        break;
        default:
        break;        
      }
      char requestingData   = buf[2];
      if (requestingData == '1'){
        transmitData()
      }
      Serial.print()
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, LOW);
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
    delay(200);
  }
  //init motors
  pinMode(S1, OUTPUT)
  pinMode(S2, OUTPUT)
  digitalWrite(S1, 0)
  digitalWrite(S2, 0)
  pinMode(TMP, INPUT_PULLUP) //temp
  pinMode(A3, INPUT_PULLUP) //door
}

void loop() {
  transmitter();
  delay(1000);
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    receiver();
  }
}

}
