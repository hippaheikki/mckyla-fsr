#!/usr/bin/env python
import cgi
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
f = open("users.txt", "rb")
cur_file = f.read()
user_list = cur_file.split("^")
f.close()
form = cgi.FieldStorage()
new_user = form.getvalue("new_user")
add_user = True
for u in user_list:
    u_name = u.split(":")[0]
    if (u_name == new_user):
        add_user = False
        break

if (len(new_user) < 1):
    print '''ERROR: No username provided'''
    add_user = False

if (add_user):
    f = open("users.txt", "wb")
    f.write(cur_file.strip('\n')+"^"+new_user+":200:200:200:200")
    f.close()
    print "Added new user \"" + new_user + "\""
    print "<a href=/pad2/pads.py?cur_user="+new_user+">Edit sensitivities</a>"
    print '''<script>setTimeout(function() { window.location = "pads.py?cur_user=%s" }, 1000) </script>''' % new_user
else:
    print "Error adding user. (Does this user already exist?)"
    print "<a href=/pad2/pads.py>"
    print '''<script>setTimeout(function() { window.location = "pads.py" }, 1000) </script>'''
print '''</body>'''
print '''</html>'''
