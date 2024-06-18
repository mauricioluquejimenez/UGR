import serial
import time

#arduino = serial.Serial('/dev/ttyACM0', 9600)
arduino = serial.Serial('COM3', 9600)

string = "Leyendo el Arduino..."
print(string)

string = ""

time.sleep(2)

while 1:
    arduino.timeout = 5
    string = str(arduino.readline())

    print(string)