#!/usr/bin/env python
import serial
import cgi
print "Content-type: text/html"
print
print '''<html>'''

print '''	<head>'''
print '''		<link rel="stylesheet" type="text/css" href="styles/styles.css">'''
print '''		<script src="js/jquery-3.2.1.min.js"></script>'''
print '''		<script src="js/scripts.js"></script>'''
print '''	</head>'''

print '''	<body>'''
print '''		<div id="maincont">'''
print '''			<div style="display: table-cell; vertical-align: top; width: 120px; height: 590px">'''
print '''				<iframe src="http://groovestats.com/ECS6/googlesheets/timer.php?machine=Ian" style="width:100px; height: 590px; border-radius: 30px;"></iframe>'''
print '''			</div>'''
print '''			<div style="display: table-cell;">'''
print '''				<div class="mainhead"><img src="img/smallpad.png" style="margin-right: 20px;">1P on ITG-Ian</div>'''
print '''				<div>'''
print '''					<div class="hcell" style="border-top: 2px solid #020a12; border-right: 2px solid #020a12; width: 240px; display: table-cell;">'''
print '''						<form action="/pads.py" method="get">'''

print '''							<select name="cur_user">'''
f = open("users.txt", "rb")
user_list = f.read().split("^")
f.close()

for u in user_list:
    u_name = u.split(":")[0]
    print '''							<option value="%s">%s</option>''' % (u_name, u_name)

print ''' 							</select>'''
print '''							<input type="submit" value="Select User">'''

print '''						</form>'''
print '''					</div>'''
print '''					<div class="hcell" style="border-top: 2px solid #020a12; width: 358px; display: table-cell;">'''
print '''						<form action="/user.py" method="get">'''
print '''							<input type="text" name="new_user"> <input type="submit" value="Create User">'''
print '''						</form>'''
print '''					</div>'''
print '''				</div>'''
form = cgi.FieldStorage()
cur_user = form.getvalue("cur_user")
#s = serial.Serial("/dev/ttyACM0", 9600)
#s.setDTR(1)
if (cur_user != "Guest" and cur_user != ""):
    for u in user_list:
        u_array = u.split(":")
        if u_array[0] == cur_user:
#            s.write("0"+u_array[1]+"\r\n")
#            s.read(78)
#            s.write("1"+u_array[2]+"\r\n")
#            s.read(78)
#            s.write("2"+u_array[3]+"\r\n")
#            s.read(78)
#            s.write("3"+u_array[4]+"\r\n")
#            s.read(78)
            break
#s.write("7\r\n")
#cur_pressures = s.read(78).split(',')
cur_pressures = [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100]
#s.close()


f = open("indexbottom.html", "rb")
print f.read() % (cur_user, int(cur_pressures[3]), int(cur_pressures[1]), int(cur_pressures[5]), int(cur_pressures[7]))
f.close()
