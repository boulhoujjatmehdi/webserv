#!/usr/bin/env python3

import cgi
import datetime

print("Content-Type: text/html")    # HTML is following
print()                             # blank line, end of headers

print("<html>")
print("<head>")
print("<title>My Complex HTML Response</title>")
print("</head>")
print("<body>")
print("<h1>Welcome to my CGI script!</h1>")
print("<p>Today's date is: ", datetime.datetime.now(), "</p>")

form = cgi.FieldStorage()

if "name" in form:
    print("<h2>Hello, ", cgi.escape(form["name"].value), "!</h2>")

print("<form method='get'>")
print("Name: <input type='text' name='name'> <input type='submit' value='Submit'>")
print("</form>")

print("</body>")
print("</html>")