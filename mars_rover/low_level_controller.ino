#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>

/*
 * Published topics: Wheel encoder count, battery percentage, Emergency stop button state.
 * Subscibed topics: Wheel target speed, arm joint angles & misc actuators.
*/

float r_target_vel;
float l_target_vel;
long r_encoder_count;
long l_encoder_count;

ros::NodeHandle nh;
std_msgs::Int32 r_encoder_msg;
std_msgs::Int32 l_encoder_msg;

void r_wheel_vel_Cb( const std_msgs::Float32& msg) {
  r_target_vel = msg.data;
}

void l_wheel_vel_Cb( const std_msgs::Float32& msg) {
  l_target_vel = msg.data;
}

ros::Subscriber<std_msgs::Float32> r_sub("r_wheel_vel", &r_wheel_vel_Cb );
ros::Subscriber<std_msgs::Float32> l_sub("l_wheel_vel", &l_wheel_vel_Cb );

ros::Publisher r_encoder_broadcaster("lwheel_ticks", &r_encoder_msg);
ros::Publisher l_encoder_broadcaster("rwheel_ticks", &l_encoder_msg);

void setup() {
  nh.initNode();
}

void loop() {
  nh.spinOnce();

  r_encoder_msg.data = r_encoder_count;
  l_encoder_msg.data = l_encoder_count;
  r_encoder_broadcaster.publish(&r_encoder_msg);
  l_encoder_broadcaster.publish(&l_encoder_msg);

  nh.loginfo("LOL"); //Use instead of Serial.print
}
