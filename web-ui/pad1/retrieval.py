#!/usr/bin/env python

import cgi
import serial
import time
print "Content-type: text/html"
print
print '''<html>'''

print '''<head>'''
print '''<link rel="stylesheet" type="text/css" href="../styles/styles.css">'''
print '''<script src="../js/jquery-3.2.1.min.js"></script>'''
print '''<script src="../js/scripts.js"></script>'''
print '''</head>'''

print '''<body>'''
print

form = cgi.FieldStorage()
cur_user = form.getvalue("cur_user")
l_pressure = form.getvalue("left_pressure")
u_pressure = form.getvalue("up_pressure")
r_pressure = form.getvalue("right_pressure")
d_pressure = form.getvalue("down_pressure")
s = serial.Serial("/dev/ttyACM0", 9600)
s.setDTR(1)
f = open("users.txt", "rb")
users_file = f.read()
f.close()
user_list = users_file.split("^")
cur_user_list_index = -1
for u in range(len(user_list)):
    if user_list[u].split(":")[0] == cur_user:
        cur_user_list_index = u
        break
cur_user_list = user_list[cur_user_list_index].strip("\n").split(":")
if (len(l_pressure) == 3):
    if cur_user_list_index != -1:
        cur_user_list[1]=l_pressure
    s.write("0"+l_pressure+"\r\n")
    new_pressures = s.read(78)
if (len(u_pressure) == 3):
    if cur_user_list_index != -1:
        cur_user_list[2]=u_pressure
    s.write("1"+u_pressure+"\r\n")
    new_pressures = s.read(78)
if (len(r_pressure) == 3):
    if cur_user_list_index != -1:
        cur_user_list[3]=r_pressure
    s.write("2"+r_pressure+"\r\n")
    new_pressures = s.read(78)
if (len(d_pressure) == 3):
    if cur_user_list_index != -1:
        cur_user_list[4]=d_pressure
    s.write("3"+d_pressure+"\r\n")
    new_pressures = s.read(78)
if (len(l_pressure) != 3 and len(u_pressure)  != 3 and len(r_pressure)  != 3 and len(d_pressure)  != 3):
    s.write("7\r\n")
    new_pressures = s.read(78)
print new_pressures.replace(",", "|")
print '''<br><a href=pads.py?cur_user=%s>Return</a>''' % cur_user

user_list[cur_user_list_index] = ":".join(cur_user_list)
f = open("users.txt", "wb")
f.write("^".join(user_list))
f.close

s.close()
print '''<script>setTimeout(function() { window.location = "pads.py?cur_user=%s" }, 1000) </script>''' % cur_user

print '''</body>'''
print '''</html>'''
