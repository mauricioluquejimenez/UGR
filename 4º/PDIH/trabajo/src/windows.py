import serial
import time

arduino = serial.Serial('COM3',9600)  # COMxx   format on Windows
                                            # /dev/ttyUSBx format on Linux
                                            #
                                            # Eg /dev/ttyUSB0
                                            # SerialObj = serial.Serial('/dev/ttyUSB0')

time.sleep(3)                             # Only needed for Arduino,For AVR/PIC/MSP430 & other Micros not needed
                                            # opening the serial port from Python will reset the Arduino.
                                            # Both Arduino and Python code are sharing Com11 here.
                                            # 3 second delay allows the Arduino to settle down.


arduino.timeout = 3                      #set the Read Timeout
string = arduino.readline()              #readline reads a string terminated by \n

print(string)

arduino.close()   