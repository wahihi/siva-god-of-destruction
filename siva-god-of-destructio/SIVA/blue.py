#! /usr/bin/python

import serial

bluetoothSerial = serial.Serial( "/dev/rfcomm1", baudrate=9600 )

tree = 0
while tree < 1:
        tree = tree + 1
        a = raw_input( "Please enter the first number: " )
        bluetoothSerial.write( "{0}".format( a ) )
#       print bluetoothSerial.readline()

#       bluetoothSerial.flush()

        bluetoothSerial.write( "{0}".format( a ) )
#       print bluetoothSerial.readline()

#       bluetoothSerial.flush()

#       ack_check = 0
#       while ack_check < 13:
#               print(bluetoothSerial.read())
#               ack_check = ack_check + 1

        headerSize = int(bluetoothSerial.read())
        print(headerSize)

        if headerSize == 1:
                msgSize = int(bluetoothSerial.read())
        elif headerSize ==2:
                msgSize = bluetoothSerial.read()
                msgSize += bluetoothSerial.read()
                msgSize = int(msgSize)

        print(msgSize)

        recMsg = ""
        for i in range(msgSize):
                recMsg += bluetoothSerial.read()
                print(recMsg)



