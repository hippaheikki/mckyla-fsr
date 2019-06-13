#!/usr/bin/env python
 
import BaseHTTPServer
import CGIHTTPServer
import cgitb
cgitb.enable()  ## This line enables CGI errorreporting

name = ""
port = 80

server = BaseHTTPServer.HTTPServer
handler = CGIHTTPServer.CGIHTTPRequestHandler
server_address = (name, port)
handler.cgi_directories = ["/pad1", "/pad2", "/cgi-bin"]

if name == "":
  name = "localhost"
print "Staring server. Visit http://%s:%s" % (name, port)
 
httpd = server(server_address, handler)
httpd.serve_forever()
