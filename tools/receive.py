#!/usr/bin/python

# Basic serial reader for linux, that takes values out of FrskyD_sensor_demo.ino and show them on screen.
#
#
# origin: https://github.com/jcheger/frsky-arduino
# author: Jean-Christophe Heger <jcheger@ordinoscope.net>

import os
import pprint
import re

import serial

ser = serial.Serial ('/dev/ttyUSB0', 115200)
pp = pprint.PrettyPrinter (indent=4)
d = {}

while True:
  l = ser.readline()
  if l:
    m = re.search ('(.*):\s+(.*)\s', l)
    if m:
        d[m.group(1)] = m.group(2)
        #os.system('cls')       # Windows
        os.system('clear')      # Linux
        pp.pprint (d)
