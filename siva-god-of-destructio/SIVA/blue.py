#! /usr/bin/python

import serial

bluetoothSerial = serial.Serial( "/dev/rfcomm1", baudrate=9600 )

tree = 0
recMsg = ""

while tree < 20:
        tree = tree + 1
        recMsg = ""
        a = raw_input( "Please enter the first number: " )
        bluetoothSerial.write( "{0}".format( a ) )

        headerSize = int(bluetoothSerial.read())
        print(headerSize)

        if headerSize == 1:
                msgSize = int(bluetoothSerial.read())
        elif headerSize ==2:
                msgSize = bluetoothSerial.read()
                msgSize += bluetoothSerial.read()
                msgSize = int(msgSize)

        print(msgSize)

        for i in range(msgSize):
                recMsg += bluetoothSerial.read()

        print(recMsg)



