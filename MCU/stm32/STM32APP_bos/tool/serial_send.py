#-*- coding:utf-8 -*-
#
import serial
import time
ser = serial.Serial('/dev/ttyUSB0',115200,parity=serial.PARITY_NONE)
#ser.open()
ser.write('c')
time.sleep(0.010)
ser.write('update')

