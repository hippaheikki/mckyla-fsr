#!/usr/bin/env python

import cgi
import serial

def getSerialConnection(padSideByteString):
	padSideStr = "0" if (padSideByteString == "left") else "1"

	s = serial.Serial("/dev/ttyACM0", 9600)
	s.setDTR(1)

	#Send 9: Gief pad side from ttyACM0
	s.write("9\r\n")
	padSide = s.readline()

	if padSide[0] != padSideStr:
		#Turns out he was the other side, so ttyACM1 has our pad! We connect to him now!
		s.close()

		s = serial.Serial("/dev/ttyACM1", 9600)
		s.setDTR(1)
	
	return s

s = getSerialConnection("left")
s.write("D\r\n")
s.close()

form = cgi.FieldStorage()
cur_user = form.getvalue("cur_user")

print "Content-type: text/html"
print
print '''<html>'''

print '''<head>'''
print '''<link rel="stylesheet" type="text/css" href="../styles/styles.css">'''
print '''<script src="../js/jquery-3.2.1.min.js"></script>'''
print '''<script src="../js/scripts.js"></script>'''
print '''</head>'''

print '''<body>'''
print '''<br><a href=pads.py?cur_user=%s>Return to main page</a>''' % cur_user

print '''<script>setTimeout(function() { window.location = "pads.py?cur_user=%s" }, 1000) </script>''' % cur_user

print '''</body>'''
print '''</html>'''

