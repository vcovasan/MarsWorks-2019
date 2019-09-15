from time import sleep\
import serial\
ser = serial.Serial('/dev/tty.usbmodem1d11', 9600)
counter = 32
while True:
     counter = counter + 1\
     ser.write(str(chr(counter)))
     print ser.readline()
     sleep(.1)
     if counter == 255:\
     counter = 32\
}
