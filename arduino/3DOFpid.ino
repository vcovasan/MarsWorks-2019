#include <FastPID.h>

/********************************/
//*******READ CAREFULLY*********
/********************************/

//Connect the actuator potentiometer cables to the SERVO 1 and SERVO 2 connectors on the PCB.
//Connect the potentiometers used to control the actuators to the SERVO 5 and SERVO 6 connectorson the PCB.
//Connect the end effector servo to the SERVO 3 connector on the PCB.
//Connect the DIRECTION and STEP pins on the STEPPER1 PCB connector to the DIR1 and SPEED1 connections respectively on the DC motor controller.
//Connect the DIRECTION and STEP pins to the STEPPER2 PCB connector to the DIR2 and SPEED2 connections respectively on the DC motor controller.
//Double check all connections.
//Upload this code to the Arduino Nano.

//Power on the H-Bridge with a +12V power supply (make sure to common the ground connection to the control PCB).
//You should now be able to control the linear actuators with the external potentiometers.
//You can connect a third potentiometer to control the end effector if you wish.

//Pin Layout

/*
  FastPID Library: https://github.com/mike-matera/FastPID

  TODO:
  - Take 4/8 readings of potentiometer feedback from linear actuators and use bit shifts for division
  - Use FastPID library and tune the PID controller
  - Servo library uses Timer/Counter 1 - BUT this code for driving Motors at a much higher PWM frequency (31KHz)
  - May need to measure how long it takes to execute the loop to adjust the PID frequency for calling the step() function
  -  To measure how long the loop takes use micros() or millis() from start of loop to the end - Use delay to increase loop time
  - Use Serial to set the motor setpoint

  For the Linear Actuators:
  250mm --> 400 steps with 10 bit ADC
  1mm -  1.6 steps (2 steps)

*/
#define MOTOR_ENABLE 3
#define IN1 2
#define IN2 6

#define outputA 4
#define outputB 7

const uint8_t LINEAR_ACTUATOR_3 = 1;
const uint8_t LINEAR_ACTUATOR_2 = 2;
const uint8_t BASE_MOTOR = 3;

// Linear Actuator Position Feedback
#define SERVO_3_FEEDBACK A4
#define SERVO_2_FEEDBACK A5

// Arm Servo's
#define ARM_SERVO_3 A2
#define ARM_SERVO_2 A1
#define ARM_SERVO_3 A0

// DC Motor Control Drive Signal Pins
#define MOTOR_3_DIR 8
#define MOTOR_3_PWM 5

#define MOTOR_2_DIR A3
#define MOTOR_2_PWM 9

// Stepper Motor (Not used anymore)
#define STEP_3 6 // PWM Pin
#define DIR_3 7

// #define STEP_4 8 // Wrong PIN!
// #define DIR_4 9 // PWM Pin

//Internal potentiometer value at min/max distance
#define ACT_1_MAX 450
#define ACT_1_MIN 50

#define ACT_2_MAX 713
#define ACT_2_MIN 312

#define ACT_3_MAX 454
#define ACT_3_MIN 52

// Function Prototypes


// Setup PID Parameters
float kp = 10, ki = 0, kd = 0, frequency = 10;
int output_bits = 9;
bool output_signed = true;

int actuator3_setpoint = 400;
int actuator2_setpoint = 500;

FastPID PID_actuator3(kp, ki, kd, frequency, output_bits, output_signed);
FastPID PID_actuator2(kp, ki, kd, frequency, output_bits, output_signed);

void setup() {

  // Setup USB-Serial to look at the messages coming back over Serial
  Serial.begin(115200);
  Serial.println("Welcome to the arm control code");
  Serial.println("Send your commands like for Linear Actuators");
  Serial.println("First Number should be between: 312 and 713");
  Serial.println("Second Number should be between: 52 and 454");

  // Set PWM Frequency 31KHz PWM to prevent motor noise
  // NOTE - Timer 1 is used by Arduino Servo Library
  TCCR1B = TCCR1B & 0b11111000 | 1;

  // end effector servo
  //UPDOWN.attach(SERVO_3);

  // Linear Actuator Feedback
  pinMode(SERVO_3_FEEDBACK, INPUT);
  pinMode(SERVO_2_FEEDBACK, INPUT);

  // DC Motor Drive Control Signals
  pinMode(MOTOR_3_DIR, OUTPUT);
  pinMode(MOTOR_3_PWM, OUTPUT);

  pinMode(MOTOR_2_DIR, OUTPUT);
  pinMode(MOTOR_2_PWM, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);

  // Set direction control signal HIGH - What mode does this set this up for?
  //  digitalWrite(MOTOR_3_DIR, HIGH);
  //  digitalWrite(MOTOR_2_DIR, HIGH);

  digitalWrite(MOTOR_3_PWM, 0);
  digitalWrite(MOTOR_2_PWM, 0);
  //
  PID_actuator3.setOutputRange(-255, 255);
  PID_actuator2.setOutputRange(-255, 255);

  delay(3000);

}

void loop() {
  // Min - At bottom
  // Blue - 312 actuator 2
  // Red 450 - actuator 3

  // red - 125 - actuator 3 (actuator 2 in code)
  // blue - 700 - actuator 2 (actuator 1 in code)


  // Measure actual position of actuator
  uint16_t actuator2_feedbackPos = analogRead(SERVO_2_FEEDBACK);
  uint16_t actuator3_feedbackPos = analogRead(SERVO_3_FEEDBACK);

  uint16_t actuator2_error = abs(actuator2_feedbackPos - actuator2_setpoint);
  uint16_t actuator3_error = abs(actuator3_feedbackPos - actuator3_setpoint);

  int16_t motor2_pwmVal = 0;
  int16_t motor3_pwmVal = 0;
  
  // Calculate control signal for actuator DC Motors
  if (actuator2_error < 10) {
    motor2_pwmVal = 0;
    PID_actuator2.clear();
  }
  else {
    motor2_pwmVal = PID_actuator2.step(actuator2_setpoint, actuator2_feedbackPos);
  }

  if (actuator3_error < 10) {
    motor3_pwmVal = 0;
    PID_actuator3.clear();
  }
  else {
    motor3_pwmVal = -(PID_actuator3.step(actuator3_setpoint, actuator3_feedbackPos));
  }

  // Print out serial to view in Serial plotter (Control Signal)
  //  Serial.print(actuator2_feedbackPos);
  //  Serial.print(" ");
  Serial.print(actuator2_feedbackPos);
  Serial.print(" ");
  Serial.print(motor2_pwmVal);
  Serial.print(" ");
  Serial.print(actuator3_feedbackPos);
  Serial.print(" ");
  Serial.print(motor3_pwmVal);
  Serial.print(" ");
  Serial.println(0);

  // Drive DC Motors using PWM Control Signal
  DriveActuatorMotor(LINEAR_ACTUATOR_3, motor3_pwmVal); // actuator 2 blue
  DriveActuatorMotor(LINEAR_ACTUATOR_2, motor2_pwmVal);  //actuator 3 - red


DriveActuatorMotor(3, -100);
//DriveActuatorMotor(3, 100);
delay(10000);
DriveActuatorMotor(3, 100);
delay(10000);
  // Delay the loop to approximate a 10Hz loop (100ms) / 100Hz loop - 10ms
  delay(100);

}

void DriveActuatorMotor(uint8_t motorCode, int16_t motorPWM) {

  switch (motorCode) {

    case 1:
      if (motorPWM >= 0) {
        // Drive Motor 1 Forward
        digitalWrite(MOTOR_3_DIR, HIGH);
        analogWrite(MOTOR_3_PWM, motorPWM);

      }
      else {
        // Drive Motor 1 Backward
        digitalWrite(MOTOR_3_DIR, LOW);
        analogWrite(MOTOR_3_PWM, abs(motorPWM));

      }
      break;

    case 2:
      if (motorPWM >= 0) {
        // Drive Motor 2 Forward
        digitalWrite(MOTOR_2_DIR, HIGH);
        analogWrite(MOTOR_2_PWM, motorPWM);

      }
      else {
        // Drive Motor 2 Backward
        digitalWrite(MOTOR_2_DIR, LOW);
        analogWrite(MOTOR_2_PWM, abs(motorPWM));
      }
      break;
      
      case 3:
      if (motorPWM >= 0) {
        // Drive Motor 1 Clockwise
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW);
    analogWrite(MOTOR_ENABLE, motorPWM);

      }
      else {
        // Drive Motor 2 Backward
        digitalWrite(IN1, LOW); //CW 
        digitalWrite(IN2, HIGH);
        analogWrite(MOTOR_ENABLE, abs(motorPWM));
      }
      break;

  }

}
