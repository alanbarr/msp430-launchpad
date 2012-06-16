import serial
import time

NEWLINE = "\r\n"

def validateCommand(lines):
    print "Length of lines:" + str(len(lines))
    error = -1
    for line in lines:
        if "ERROR:" in line:
            line = line.rstrip()
            error = int(line[6:])
    print "Validate:" + str(error)
    return error

def waitToSend(lines):
    readyFound = False
    for line in lines:
        if "R" in line:
            readyFound = True
            break
    return readyFound

ser = serial.Serial("/dev/ttyACM0", 9600, timeout = 0.5)
ser.write("r" + NEWLINE)
lines = ser.readlines()
print lines
validateCommand(lines)
while waitToSend(ser.readlines()) == False:
    continue

ser.write("t00" + NEWLINE)
lines = ser.readlines()
validateCommand(lines)

ser.write("t00" + NEWLINE)
lines = ser.readlines()
validateCommand(lines)

time.sleep(1)
ser.write("t03" + NEWLINE)
lines = ser.readlines()
validateCommand(lines)



ser.close()
