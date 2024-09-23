import serial
import time

#arduino = serial.Serial('/dev/ttyACM0', 9600)
arduino = serial.Serial('COM3', 9600)
time.sleep(2)

while 1:
    var = input('\n Pulsa 1 para encender el LED y 0 para apagarlo: ')
    print(" Has introducido", var)

    if(var == '1'):
        arduino.write(var.encode())
        print(" LED encendido")
        time.sleep(1)

    elif(var == '0'):
        arduino.write(var.encode())
        print(" LED apagado")
        time.sleep(1)
    
    else:
        print("\n Valor incorrecto")

