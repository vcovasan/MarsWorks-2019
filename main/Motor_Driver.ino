#include <FastPID.h>

#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>

#define PWM1  10
#define DIR1  12
#define EN1   4
#define BRK1  6
#define FLT1  8
#define PWM2  13
#define DIR2  3
#define EN2   30
#define BRK2  2
#define FLT2  11

#define ID  0x1BEEF003
#define HOSTID 0x1DDDDDDE

unsigned char len = 0;
unsigned char buf[8];
unsigned char stmp[8] = {0x0E, 0x00, 0xFF, 0x22, 0xE9, 0xFA, 0xDD, 0x51};
const int SPI_CS_PIN = 17;

const int SwStA[2][2][2] = {{{0,2},{4,3}},{{6,1},{5,0}}};
const int SwStB[2][2][2] = {{{6,1},{0,2}},{{5,0},{4,3}}};

volatile long tickCount1 = 0;
volatile int oldSwSt1 = 1;
int errorFlag = 0;
  int deltaT = 1, deltaTick1 = 1;
  float tickedSpeed1, setSpeed1 = 0.5;

volatile long tickCount2 = 0;
volatile int oldSwSt2 = 1;

  int deltaTick2 = 1;
  float tickedSpeed2, setSpeed2 = 0.5;

const float Kp=2.0, Ki=10.0, Kd=0, Hz=10; //<---------------------PID Settings
const int output_bits = 16;
const bool output_signed = true;

MCP_CAN CAN(SPI_CS_PIN);
FastPID myPID1(Kp, Ki, Kd, Hz, output_bits, output_signed);
FastPID myPID2(Kp, Ki, Kd, Hz, output_bits, output_signed);

void setup() {
  //Serial1.begin(115200);
  pinMode(PWM1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(BRK1, OUTPUT);
  digitalWrite(PWM1, LOW);
  digitalWrite(DIR1, HIGH);
  digitalWrite(EN1, HIGH);
  digitalWrite(BRK1, HIGH);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(FLT1, INPUT_PULLUP);

  pinMode(PWM2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(BRK2, OUTPUT);
  digitalWrite(PWM2, LOW);
  digitalWrite(DIR2, LOW);
  digitalWrite(EN2, HIGH);
  digitalWrite(BRK2, HIGH);
  pinMode(FLT2, INPUT_PULLUP);

  if(CAN.begin(CAN_250KBPS) == CAN_OK){
    analogWrite(PWM1, 100);
  }
  else
    analogWrite(PWM1, 0);

  TCCR1B = (TCCR1B & 0b11111000) | 0x02;

  //timer ISR stuff
  OCR0A = 180; // Every 1000 µs approximately
  TIMSK0 |= _BV(OCIE0A);

  delay(2000);
}  


SIGNAL(TIMER0_COMPA_vect){
  int SwSt1 = SwStA[digitalRead(A3)][digitalRead(A4)][digitalRead(A5)];
  int SwSt2 = SwStA[digitalRead(A0)][digitalRead(A1)][digitalRead(A2)];
  int dir1, dir2;

  if(SwSt1 == 0 || SwSt2 == 0){
    errorFlag = 1;
  }
  
  dir1 = oldSwSt1 - SwSt1;
  dir2 = oldSwSt2 - SwSt2;
  dir1 += (dir1 == 5) ? -4 : ((dir1 == -5) ? 4 : 0); //Removes erroneous subtract (like from going from 6 to 1 or vice versa
  dir2 += (dir2 == 5) ? -4 : ((dir2 == -5) ? 4 : 0);
  tickCount1 += dir1;
  tickCount2 += dir2;
  oldSwSt1 = SwSt1;
  oldSwSt2 = SwSt2;
  
} 

int startup1(void){
  int s1 = digitalRead(A3);
  int s2 = digitalRead(A4);
  int s3 = digitalRead(A5);
  for(int i = 0; i < 255; i++){
    analogWrite(PWM1, i);
    delay(10);
  }
  
  while(digitalRead(A3) == s1 && digitalRead(A4) == s2 && digitalRead(A5) == s3);
  
}

int j = 0;
int i = 0;
void loop() {
  long oldTickCount1, oldTickCount2;
  unsigned long oldTime;
  long averageTickCount;
  
  oldTime = millis();
  oldTickCount1 = tickCount1;
  oldTickCount2 = tickCount2;
  
  averageTickCount = (oldTickCount1 + oldTickCount2)/2;
  
  tickedSpeed1 = ((float)deltaTick1 / (float)deltaT)*10.82;
  tickedSpeed2 = ((float)deltaTick2 / (float)deltaT)*10.82;

  char sendbuf[8];
  int temp;
  if (CAN_MSGAVAIL == CAN.checkReceive())           // check if data coming
  {
    //if(HOSTID == CAN.getCanId())
   // Serial1.println(CAN.getCanId());
    {
      sendbuf[8] = "\0\0\0\0\0\0\0";
      temp = 0;
      CAN.readMsgBuf(&len, (void*)(&temp));    // read data,  len: data length, buf: data buf
      
      setSpeed1 = (float)temp / 1000.0;
    }
  }
  
  
  if(i++ == 10){
    i = 0;
    j++;
    char tempshit[8] = "FFS";
    CAN.sendMsgBuf(ID, 1, sizeof(averageTickCount), (void*)(&averageTickCount));   
    //setSpeed1 = (float)j / 10.0;
  }

  //PID
  
  int16_t pwm1 = myPID1.step(setSpeed1, -tickedSpeed1);
  int16_t pwm2 = myPID2.step(setSpeed1, -tickedSpeed2);
  
  //Bounds checking
  if (pwm1 < 0){
    pwm1 *= -1;
    digitalWrite(DIR1, HIGH);
  }
  else {
    if(pwm1 > 255){
      pwm1 = 255;
    }
    digitalWrite(DIR1, LOW);
  }

  if (pwm2 < 0){
    pwm2 *= -1;
    digitalWrite(DIR2, HIGH);
  }
  else{
    if(pwm2 > 255){
      pwm2 = 255;
    }
    digitalWrite(DIR2, LOW);
  }
  
  //do stuff that takes a while
  delay(50);
  //startup1();

  analogWrite(PWM1, pwm1);
  analogWrite(PWM2, pwm2);
    
//  for (i = 0; i < 255; i++) {
//    analogWrite(PWM1, i);
//    delay(50);
//  }
//  for (i = 254; i > 0; i--) {
//    analogWrite(PWM1, i);
//    delay(50);
//  }  
  deltaT = millis() - oldTime;
  deltaTick1 = tickCount1 - oldTickCount1;
  deltaTick2 = tickCount2 - oldTickCount2;
}
