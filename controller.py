import PySimpleGUI as sg
import pygame
import xbox360_controller
import serial
import time

# Track Joint Positions as Globals - Start with ones on the rover first (from doing a analogRead()) - Then just increment and decrement!
# Use a window_handler() function to handle things window.read(timeout=100)

ser = serial.Serial('COM4', 115200)

# Joint Positions
joint1_pos = 10
joint2_pos = 10
joint3_pos = 10
joint4_pos = 10
joint5_pos = 10
joint6_pos = 10

# Constants for Joysticks
LEFT_STICK_LEFT = 1
LEFT_STICK_RIGHT = 2
LEFT_STICK_UP = 3
LEFT_STICK_DOWN = 4

RIGHT_STICK_LEFT = 5
RIGHT_STICK_RIGHT = 6
RIGHT_STICK_UP = 7
RIGHT_STICK_DOWN = 8

A_BUTTON_PRESSED = 9
B_BUTTON_PRESSED = 10
X_BUTTON_PRESSED = 11
Y_BUTTON_PRESSED = 12

BACK_BUTTON_PRESSED = 13
START_BUTTON_PRESSED = 14
LT_BUMP_BUTTON_PRESSED = 15
RT_BUMP_BUTTON_PRESSED = 16
LT_STICK_BUTTON_PRESSED = 17
RT_STICK_BUTTON_PRESSED = 18
GUIDE_BUTTON_PRESSED = 27

LEFT_TRIGGER = 19
RIGHT_TRIGGER = 20

PAD_UP_PRESSED = 21
PAD_DOWN_PRESSED = 22
PAD_RIGHT_PRESSED = 23
PAD_LEFT_PRESSED = 24

LEFT_STICK = 25
RIGHT_STICK = 26

JOYSTICK_THRESHOLD = 0.2
TRIGGER_THRESHOLD = 0.2

# PyGame Setup for Xbox 360 Controller
pygame.init()
size = [100, 100]
screen = pygame.display.set_mode(size)
pygame.display.set_caption("X-Box 360 Controller")
FPS = 10
clock = pygame.time.Clock()

# make a controller (should this be in the game loop?)
controller = xbox360_controller.Controller(0)
print(dir(controller))

# Rover Mode
roverMode = 1 # Rover Mode --> Waiting (0) | Driving (1) | Arm (2) | Measurement (Requesting Data and Opening/Closing Boxes) (3)
oldStartState = 0
def change_mode():
    buttonStates = controller.get_buttons();
    start_button = buttonStates[xbox360_controller.START]
	
    global roverMode
    global oldStartState
    if start_button != oldStartState:
        if (start_button == 1):
            roverMode = roverMode + 1
            # Reset rover mode
            if(roverMode > 4):
                    roverMode = 1
    oldStartState = start_button
    
def button_handler():
    buttonStates = controller.get_buttons();
    a_button = buttonStates[xbox360_controller.A]
    b_button = buttonStates[xbox360_controller.B]
    x_button = buttonStates[xbox360_controller.X]
    y_button = buttonStates[xbox360_controller.Y]
    back_button = buttonStates[xbox360_controller.BACK]
    
    start_button = buttonStates[xbox360_controller.START]   
    lt_bump_button = buttonStates[xbox360_controller.LEFT_BUMP]
    rt_bump_button = buttonStates[xbox360_controller.RIGHT_BUMP]
    lt_stick_button = buttonStates[xbox360_controller.LEFT_STICK_BTN]
    rt_stick_button = buttonStates[xbox360_controller.RIGHT_STICK_BTN]

    if(a_button == 1):
        return A_BUTTON_PRESSED
    elif(b_button == 1):
        return B_BUTTON_PRESSED
    elif(x_button == 1):
        return X_BUTTON_PRESSED
    elif(y_button == 1):
        return Y_BUTTON_PRESSED
    elif(back_button == 1):
        return BACK_BUTTON_PRESSED
    elif(start_button == 1):
        return START_BUTTON_PRESSED
    elif(lt_bump_button == 1):
        return LT_BUMP_BUTTON_PRESSED
    elif(rt_bump_button == 1):
        return RT_BUMP_BUTTON_PRESSED
    elif(lt_stick_button == 1):
        return LT_STICK_BUTTON_PRESSED    
    elif(rt_stick_button == 1):
        return RT_STICK_BUTTON_PRESSED
    
def joystick_handler():

    left_stick_x, left_stick_y = controller.get_left_stick()
    right_stick_x, right_stick_y = controller.get_right_stick()		

    # Left Joystick
    if (left_stick_x > JOYSTICK_THRESHOLD):
    	# Right
    	return LEFT_STICK_RIGHT, abs(left_stick_x)

    elif (left_stick_x < -JOYSTICK_THRESHOLD):
    	# Left
    	return LEFT_STICK_LEFT, abs(left_stick_x)	

    elif (left_stick_y > JOYSTICK_THRESHOLD):
    	# Up
    	return LEFT_STICK_UP, abs(left_stick_y)

    elif (left_stick_y < -JOYSTICK_THRESHOLD):
    	# Up
    	return LEFT_STICK_DOWN, abs(left_stick_y)
   	
   	# Right Joystick
    elif (right_stick_x > JOYSTICK_THRESHOLD):
    	# Right
    	return RIGHT_STICK_RIGHT, abs(right_stick_x)

    elif (right_stick_x < -JOYSTICK_THRESHOLD):
    	# Left
    	return RIGHT_STICK_LEFT, abs(right_stick_x)

    elif (right_stick_y > JOYSTICK_THRESHOLD):
    	# Up
    	return RIGHT_STICK_UP, abs(right_stick_y)

    elif (right_stick_y < -JOYSTICK_THRESHOLD):
    	# DOWN
    	return RIGHT_STICK_DOWN, abs(right_stick_y)

    else:
        return 0, 0
    
# def left_joystick_handler():
    
#     left_stick_x, left_stick_y = controller.get_left_stick() 
#     # Left Joystick
#     if (left_stick_y > JOYSTICK_THRESHOLD):
#     	# Up
#     	return LEFT_STICK_UP, abs(left_stick_y)

#     elif (left_stick_y < -JOYSTICK_THRESHOLD):
#     	# Up
#     	return LEFT_STICK_DOWN, abs(left_stick_y)

#     else:
#         return 0, 0

# def right_joystick_handler():
    
#     right_stick_x, right_stick_y = controller.get_right_stick()
#     # Right Joystick
#     if (right_stick_y > JOYSTICK_THRESHOLD):
#     	# Up
#     	return RIGHT_STICK_UP, abs(right_stick_y)

#     elif (right_stick_y < -JOYSTICK_THRESHOLD):
#         # DOWN
#         return RIGHT_STICK_DOWN, abs(right_stick_y)

#     else:
#         return 0, 0
    
# define the layout of the window here with all the buttons!
def trigger_handler():
    triggers = controller.get_triggers()

    if(triggers > TRIGGER_THRESHOLD):
        return RIGHT_TRIGGER
    elif(triggers < -TRIGGER_THRESHOLD):
        return LEFT_TRIGGER

def dpad_handler():
    pad_up, pad_right, pad_down, pad_left = controller.get_pad()

    if(pad_up == 1):
        return PAD_UP_PRESSED
    if(pad_down == 1):
        return PAD_DOWN_PRESSED
    if(pad_left == 1):
        return PAD_LEFT_PRESSED
    if(pad_right == 1):
        return PAD_RIGHT_PRESSED

def rover_driving_controller():
    pass
    
def arm_control():
    pass

def joint1_control(joystick):
    # Joint 1 - Left Joystick left and Right
    global joint1_pos
    if(joystick[0] == LEFT_STICK_LEFT):
        joint1_pos += 1

    elif(joystick[0] == LEFT_STICK_RIGHT):
        joint1_pos -= 1
        
# Linear Actuator 2
def joint2_control(joystick):
    # Joint 2 - Left Joystick left and Right
    global joint2_pos
    if(joystick[0] == LEFT_STICK_UP):
        joint2_pos += 1

    elif(joystick[0] == LEFT_STICK_DOWN):
        joint2_pos -= 1

# Linear Actuator 3
def joint3_control(joystick):
    # Joint 3 - Left Joystick left and Right
    global joint3_pos
    if(joystick[0] == RIGHT_STICK_UP):
        joint3_pos += 1

    elif(joystick[0] == RIGHT_STICK_DOWN):
        joint3_pos -= 1

#  End Effector Up/Down Control
def joint4_control(dpad):
    # Joint 4 - DPad Up and Down
    global joint4_pos
    if(dpad == PAD_UP_PRESSED):
        joint4_pos += 1

    elif(joystick[0] == PAD_DOWN_PRESSED):
        joint4_pos -= 1

# End Effector Rotate
def joint5_control(joystick):
    # Joint 5 - Rotate left and right using joystick
    global joint5_pos
    if(joystick[0] == RIGHT_STICK_RIGHT):
        joint5_pos += 1

    elif(joystick[0] == RIGHT_STICK_LEFT):
        joint5_pos -= 1

# End effector 
def joint6_control(trigger):
    # Joint 6 - Open and Close Gripper
    global joint6_pos
    if(trigger == RIGHT_TRIGGER):
        joint6_pos += 1

    elif(trigger == LEFT_TRIGGER):
        joint6_pos -= 1
     
# Send 
def send_message1(messageList):
    # MessageList - Code (The instruction) | Message (E.G PWM) or Joint Position 
    serialByteArray = bytearray()
    serialByteArray.append(0xFF)
    serialByteArray.append(hex(ord(messageList[0]))) # Converts the string 
    serialByteArray.append(hex(int(messageList[1]))) # Should sent 
    serialByteArray.append(0xFF)
    ser.write(serialByteArray)
        
# Send message over serial with no start and end bytes
def send_message(messageList):
    ser.write('<'.encode())
    for i in range(0, len(messageList)):
        ser.write(messageList[i].encode())
    ser.write('>'.encode())
    
lastVelocity = 0

# Main Loop 
running = True
while running:
    # PyGame Event Loop
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Check if start button is pressed to change rover mode  
    change_mode()
    messageList = []               
    if(roverMode == 1):
        # Rover is driving - Use one set of mappings
        messageList.append('w')
        joystick = joystick_handler();
        stick = 0
        
        if(joystick[0] == LEFT_STICK_UP):
            # Go Forward
            messageList.append('f')
            stick = LEFT_STICK
            #ser.write('f'.encode())
	       
        elif(joystick[0] == LEFT_STICK_DOWN):
            # Reverse
            messageList.append('b')
            stick = LEFT_STICK
            #ser.write('b'.encode())

        elif(joystick[0] == LEFT_STICK_LEFT):
            # Turn Left
            messageList.append('l')
            stick = LEFT_STICK
            #ser.write('l'.encode())

        elif(joystick[0] == LEFT_STICK_RIGHT):
            # Turn Right
            messageList.append('r')
            stick = LEFT_STICK
            #ser.write('r'.encode())

        elif(joystick[0] == 0):
            # Not moving
            messageList.append('f')
            stick = LEFT_STICK
            #ser.write('f'.encode())

        newVelocity = round(joystick[1] * 9)
        # Check if there is enough of a change to send a new PWM value to the wheels of the rover
        if(((abs(newVelocity - lastVelocity)) > 0) and stick == LEFT_STICK):
            lastVelocity = newVelocity
            messageList.append(str(newVelocity))
            send_message(messageList) # Being used to send messages over serial
            print(newVelocity)
        else:
            # Empty the message list
            messageList = []
                    
        
    if(roverMode == 2):
       # Rover Arm is being controlled
       print("Rover arm control")
       joystick = joystick_handler()
       button = button_handler()
       trigger = trigger_handler()
       dpad = dpad_handler()
       joint1_control(joystick)
       joint2_control(joystick)
       joint3_control(joystick)
       joint4_control(dpad)
       joint5_control(joystick)
       joint6_control(trigger)
       
       
        # Check that joint value has changed from last time before sending it again!
        # If it's too fast use jointpos += 0.1 and then only send the when it is has reached an integer (use the is_integer() method)

           
    if(roverMode == 3):
        # Measurements are being taken - SerialRead with timeout!
        print("Rover Measurement Mode")
        pass

    if(roverMode == 4):
        # Switching between video channels
        print("Video Channel Switching Mode")
        pass
        

pygame.quit ()

