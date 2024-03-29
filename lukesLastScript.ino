//who needs abstractions anyway
long unsigned int motors_l_address;
long unsigned int motors_r_address;
char direction;
int mag = 0;
int pwml = 0;
int pwmr = 0;
void forwardWheelMessages() {

  //left motors
  motors_l_address = 0x1BEEF009;
 // motors_l_address = 0x1BEEF005;
  direction = (char)LoRa.read();
  mag = LoRa.read() - 48;
  if (direction == 'f' ) {
    pwml = (mag * 27);
    pwmr = (mag * 27);
  }
  else if (direction == 'b') {
    pwml = (mag * -27);
    pwmr = (mag * -27);
  }
  else if (direction == 'l') {
    pwml = (mag * -27);
    pwmr = (mag * 27);
  }
  else if (direction == 'r') {
    pwml = (mag * 27);
    pwmr = (mag * -27);
  }
  char* addressl_ptr = (void*)(&motors_l_address);
  char* pwml_ptr = (void*)(&pwml);
  char* pwmr_ptr = (void*)(&pwmr);
  Serial1.write('!');
  Serial1.write(addressl_ptr[0]);
  Serial1.write(addressl_ptr[1]);
  Serial1.write(addressl_ptr[2]);
  Serial1.write(addressl_ptr[3]);
  Serial1.write(pwml_ptr[0]);
  Serial1.write(pwml_ptr[1]);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(pwml_ptr[0]);
  Serial1.write(pwml_ptr[1]);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial3.write('!');
  Serial3.write(addressl_ptr[0]);
  Serial3.write(addressl_ptr[1]);
  Serial3.write(addressl_ptr[2]);
  Serial3.write(addressl_ptr[3]);
  Serial3.write(pwml_ptr[0]);
  Serial3.write(pwml_ptr[1]);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(pwml_ptr[0]);
  Serial3.write(pwml_ptr[1]);
  Serial3.write(0x00);
  Serial3.write(0x00);
  //right motors
  motors_r_address = 0x1BEEF005;;
  char* addressr_ptr = (void*)(&motors_r_address);
  addressr_ptr = (void*)(&motors_r_address);
  delay(50);
  Serial1.write('!');
  Serial1.write(addressr_ptr[0]);
  Serial1.write(addressr_ptr[1]);
  Serial1.write(addressr_ptr[2]);
  Serial1.write(addressr_ptr[3]);
  Serial1.write(pwmr_ptr[0]);
  Serial1.write(pwmr_ptr[1]);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(pwmr_ptr[0]);
  Serial1.write(pwmr_ptr[1]);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial3.write('!');
  Serial3.write(addressr_ptr[0]);
  Serial3.write(addressr_ptr[1]);
  Serial3.write(addressr_ptr[2]);
  Serial3.write(addressr_ptr[3]);
  Serial3.write(pwmr_ptr[0]);
  Serial3.write(pwmr_ptr[1]);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(pwmr_ptr[0]);
  Serial3.write(pwmr_ptr[1]);
  Serial3.write(0x00);
  Serial3.write(0x00);
}

long unsigned int arm_tx_address_1;
long unsigned int arm_tx_address_2;
long unsigned int arm_tx_address_3;
void forwardArmMessages(){
}

long unsigned int cam_tx_address = 0x1BEEF018;
char* cam_address_ptr = (void*)(&cam_tx_address);
void testCAMFeed(){
    Serial1.write('!');
    Serial1.write(cam_address_ptr[0]);
    Serial1.write(cam_address_ptr[1]);
    Serial1.write(cam_address_ptr[2]);
    Serial1.write(cam_address_ptr[3]);
    Serial1.write('1');
    Serial1.write('1');
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial3.write('!');
    Serial3.write(cam_address_ptr[0]);
    Serial3.write(cam_address_ptr[1]);
    Serial3.write(cam_address_ptr[2]);
    Serial3.write(cam_address_ptr[3]);
    Serial3.write('1');
    Serial3.write('1');
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
}
void testCAMFeeds(){
  for (int i = 0; i < 12; i++){
    Serial1.write('!');
    Serial1.write(cam_address_ptr[0]);
    Serial1.write(cam_address_ptr[1]);
    Serial1.write(cam_address_ptr[2]);
    Serial1.write(cam_address_ptr[3]);
    Serial1.write((char)i);
    Serial1.write((char)i);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial1.write(0x00);
    Serial3.write('!');
    Serial3.write(cam_address_ptr[0]);
    Serial3.write(cam_address_ptr[1]);
    Serial3.write(cam_address_ptr[2]);
    Serial3.write(cam_address_ptr[3]);
    Serial3.write((char)i);
    Serial3.write((char)i);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    delay(1000);
  }
}
char cam;
void forwardCameraMessage(){
  cam = (char)LoRa.read();
  Serial1.write('!');
  Serial1.write(cam_address_ptr[0]);
  Serial1.write(cam_address_ptr[1]);
  Serial1.write(cam_address_ptr[2]);
  Serial1.write(cam_address_ptr[3]);
  Serial1.write(cam);
  Serial1.write(cam);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial3.write('!');
  Serial3.write(cam_address_ptr[0]);
  Serial3.write(cam_address_ptr[1]);
  Serial3.write(cam_address_ptr[2]);
  Serial3.write(cam_address_ptr[3]);
  Serial3.write(cam);
  Serial3.write(cam);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(0x00);
}

void forwardBoxMessages() {}
