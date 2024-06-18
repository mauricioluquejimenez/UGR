import serial

SerialPortName = '/dev/ttyACM0'

SerialPortObj = serial.Serial(SerialPortName)

print(f'\n Puerto: {SerialPortName}\n')
print('\n Estado: ', SerialPortObj)

SerialPortObj.close()

print(f'\nSerial Port {SerialPortName} closed\n')