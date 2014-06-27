#!/usr/bin/python
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
        #os.system('cls')
        os.system('clear')
        pp.pprint (d)
