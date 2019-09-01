# mars_rover

mars_rover is a ROS package for controlling a rover built for the [European Rover Challenge](http://roverchallenge.eu/).

## Installation


```bash
cd ~/catkin_ws/src
git clone https://github.com/Marwan99/mars_rover.git
cd ..
catkin_make
source devel/setup.bash
```

## Usage
To view the robot model in RVIZ:
```bash
roslaunch rover_description view_model.launch
```
To run a rover simulation in Gazebo:
```bash
roslaunch rover_description rover_description.launch
```
To build a map:
```bash
roslaunch rover_navigation map_builder.launch
```
To navigate in the odometric frame:
```bash
roslaunch rover_navigation odom_nav.launch
