#! /usr/bin/python

import serial
import time

bluetoothSerial = serial.Serial( "/dev/rfcomm1", baudrate=9600 )


def sibu():
        print("sibu call")
        recMsg = ""
        a = "SIVAhumiEND"
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

def main():
    pass

if __name__ == '__main__':
        main()
        cnt = 0

        #while cnt < 5:
        while True:
                cnt += 1
                sibu()
                time.sleep(1)
