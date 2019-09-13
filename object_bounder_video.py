'''
A program to localise and detect presence of a reasonably sized green object
using fundamental image processing operations

Written by: Hari Krishna [HKrishna1@sheffield.ac.uk]
on 13/09/2019
with parts taken from Stack Overflow (ofcourse)
'''


import cv2
import numpy as np
import datetime
import traceback


# This is just for putting colors in console error messages - ignore
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def print_with_timestamp(message_type, message):
    """
    A custom made function to print error messages in a very posh way. (with a timestamp and some colors)
    :param message_type: Type of Error message
    :param message: The actual message
    """
    message_to_be_printed = bcolors.OKBLUE + "[" + str(
        datetime.datetime.now()) + "]" + bcolors.WARNING + "[" + message_type + "]" + bcolors.FAIL + "[" + str(
        message) + "]" + bcolors.ENDC
    print(message_to_be_printed)
    print(traceback.print_exc())

def is_object_visible(source):
    """
    A function that takes source of imagery as an input,
    0 for webcam OR a path to a filename if your input
    is a filename



    :param source: Source of input for the imagery, example usage below
    :return boolean.
    Returns True if green object is found
    Returns false if object is not seen in the frame grabbed
    Returns None if input source is faulty or invalid

    Source for webcam is 0, change to a string if you want to open a file
    #print(is_object_visible(source='videos/vtest1.avi'))

    #print(is_object_visible(source=0))

    """

    # Set up our input source
    cap = cv2.VideoCapture(source)

    if (cap.isOpened()):
        ret, frame = cap.read()
        # Process exactly one frame and then break
        try:
            locate_object = frame_process(frame)

            if (locate_object == True):
                cap.release()
                return True
            else:
                cap.release()
                return False
        # Happens when input is over
        except cv2.error as e:
            print_with_timestamp(message_type="End of input",
                                 message="No input detected from source. Is your camera connected?")
            print_with_timestamp(message_type="End of input",
                                 message=e)
            cap.release()
            return False

        # Handle any other unexpected exceptions
        except Exception as e:
            print_with_timestamp(message_type="Unhandled exception, see message", message=e)
            cap.release()
            return False




def main():

    # Source for webcam is 0, change to a string if you want to open a file
    #print(is_object_visible(source='videos/vtest1.avi'))

    print(is_object_visible(source=0))



def frame_process(frame):
    ''' Perform image pre-processing here such as blurring and erosion and contour analysis to determine
     if the image has our object
     :param frame: An image matrix
     :return: True if item found, False otherwise

    '''
    original_frame = frame

    # Blur

    frame = cv2.GaussianBlur(frame, (5, 5), 4)

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # You are assuming a fixed parameter called sensitivity. Depending on scenario, this can be changed.

    sensitivity = 15;
    lower_bound = [60 - sensitivity, 100, 50]  # lower bound green in HSV space
    upper_bound = [60 + sensitivity, 255, 255]  # upper bound green in HSV space

    lower = np.array(lower_bound)
    upper = np.array(upper_bound)

    # Here we are defining range of green color in HSV
    # This creates a mask of green coloured
    # objects found in the frame.
    mask = cv2.inRange(hsv, lower, upper)

    ''' Perform contour analysis on the mask '''

    (contours, _) = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # OpenCV4 compatibility fix for OpenCV3

    # contours = contours[1] if imutils.is_cv3() else contours[0]

    # Loop through each contour

    # Keep track of number of contours
    number_of_contours = len(contours)

    for contour in contours:
        print("Contour len")
        print(len(contours))

        (x, y, w, h) = cv2.boundingRect(contour)
        aspect_ratio = w / float(h)
        area = cv2.contourArea(contour)

        print("Area of contour is :" + str(area))
        print("Aspect ratio of contour is :" + str(aspect_ratio))

        # To avoid any noise
        if (area > 50):
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 4)
            coordinate_string = "(x=" + str(x) + ", y=" + str(y) + ")"
            # Screencast only
            # Text for top-left
            if (number_of_contours > 1):
                print("X")
            else:
                return True
                #cv2.putText(frame, "Object detected", (350, 25), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0))
            return False
            #cv2.putText(frame, coordinate_string, (350, 60), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255))
            '''
            
            Useless code which might be useful in the future
            # print("I see the object")

            # CSV - write
            # filename, width, height, class, xmin, ymin, xmax, ymax
            height, width, channels = original_frame.shape

            x_min = x
            y_min S= y
            x_max = x + w
            y_max = y + h

            file_name = (str(datetime.datetime.now())).replace(" ", "_").replace(".", "_").replace(":", "_")
            file_name = file_name + ".png"
            dir = "test_images/"
            full_dir_name = dir + file_name
            print(file_name)

            row = [file_name, str(width), str(height), "Obj", str(x_min), str(y_min), str(x_max), str(y_max)]

            print(row)

            with open('test_data_v1.csv', 'a') as writeFile:
                writer = csv.writer(writeFile)
                writer.writerow(row)
                cv2.imwrite(full_dir_name, original_frame)
            writeFile.close()

    # The bitwise and of the frame and mask is done so
    # that only the green coloured objects are highlighted
    # and bluestored in res
    res = cv2.bitwise_and(frame, frame, mask=mask)

    cv2.imshow('Frame', frame)
    cv2.imshow('Mask Object', mask)

    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        # Destroys all of the HighGUI windows.
        cv2.destroyAllWindows()

        # release the captured frame

        return 0
'''

if __name__ == "__main__":
    main()
