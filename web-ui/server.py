#!/usr/bin/env python
 
import BaseHTTPServer
import CGIHTTPServer
import cgitb
cgitb.enable()  ## This line enables CGI errorreporting
 
server = BaseHTTPServer.HTTPServer
handler = CGIHTTPServer.CGIHTTPRequestHandler
server_address = ("", 80)
handler.cgi_directories = ["/", "/cgi-bin"]
 
httpd = server(server_address, handler)
httpd.serve_forever()
