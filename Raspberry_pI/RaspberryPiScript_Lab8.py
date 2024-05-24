import os
import time
import serial

ser = serial.Serial("/dev/ttyS0", 9600) # mini UART is being used, 9600 is default baud rate of bluetooth module
wf_ser = serial.Serial("/dev/ttyAMA1", 115200)
ls_ser = serial.Serial("/dev/ttyAMA2", 115200)

# method is self explanatory
def clear_screen():
    if os.name == 'nt': # for windows systems
        _ = os.system('cls')
    else:               # for unix / linux / mac os
        _ = os.system('clear')

clear_screen()
    
# test to ensure communication between bluetooth module and pi is working
ser.write(bytearray('AT', 'ascii'))
time.sleep(.5)
reading = ser.read(2) # expected response is 'OK' so read two bytes
print('Communication Check: ' + str(reading))
time.sleep(.5)


# renew first, start with a clean slate
ser.write(bytearray('AT+RENEW', 'ascii'))
time.sleep(.5)
reading = ser.read(8) # expected response is 'OK+RENEW'
print('Renew: ' + str(reading))
time.sleep(.5)


# set up connection notifications
ser.write(bytearray('AT+NOTI1', 'ascii')) # when link established\lost, output CONN\LOST
time.sleep(.5)
reading = ser.read(8)
print('Notify: ' + str(reading))
time.sleep(.5)

ser.write(bytearray('AT+NOTP1', 'ascii')) # when link established\lost, output with MAC
time.sleep(.5)
reading = ser.read(8)


# set up write transmission mode
ser.write(bytearray('AT+RESP0', 'ascii')) # write without response mode
time.sleep(.5)
reading = ser.read(8)
print('Write without Response: ' + str(reading))
time.sleep(.5)


# set up complete, time to connect!
print('Connect to device...')
time.sleep(5)

reading = ser.read(8) # this will wait until you connect
print('Device Response: ' + str(reading))
if (reading != bytearray('OK+CONN:', 'ascii')):
    print('Did not connect, something went wrong. Try again!')
else:
    print('Connected successfully!')


# send data to iphone, be sure to enable notifiactions and set decode to UTF-8 on app!
while 1:
    # get temperature reading
    reading = wf_ser.read(2) # read two bytes
    temp = (reading[0] << 8) + reading[1]
    temp = temp/10
    
    reading = ls_ser.read(2) # read two bytes
    light = (reading[1] << 8) + reading[0]
    light = light/10
    
    out_str = 'T: ' + str(temp) + '    L: ' + str(light) + '%'
    ser.write(bytearray(str(out_str), 'ascii'))
