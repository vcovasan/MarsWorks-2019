#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

ros::NodeHandle  nh;
sensor_msgs::Range range_msg;
ros::Publisher pub_range( "range_data", &range_msg);

const int analog_pin = 0;
unsigned long range_timer;

float getRange(int pin_num){
  int sample;
  sample = analogRead(pin_num)/4;
  if(sample < 10) // if we're really far from anything
    return 254; 
   sample= 1309/(sample-3); // quantisation level to cm 
   return (sample - 1)/100; // in meters
}

char frameid[] = "/ir_ranger";


void setup() {

nh.initNode();
nh.advertise(pub_range);

range_msg.radiation_type = sensor_msgs::Range::INFRARED;
range_msg.header.frame_id =  frameid;
range_msg.field_of_view = 0.01;
// optionally: range_msg.min_range = 0.03;
// optionally 2: range_msg.max_range = 0.4;

}

void loop() {
// publish every 50ms 
if ( (millis()-range_timer) > 50){
  range_msg.range = getRange(analog_pin);
  range_msg.header.stamp = nh.now();
  pub_range.publish(&range_msg);
  range_timer =  millis();
}

nh.spinOnce();

}
