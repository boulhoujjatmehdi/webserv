#!/usr/bin/env python
import cgi
import os

form = cgi.FieldStorage()

name = form.getvalue('name')
email = form.getvalue('email')
message = form.getvalue('message')

print("<html>")
print("<head>")
print("<title>Form Data</title>")

print("<style>")
print("   * {")
print("    margin: 0;")
print("    padding: 0;")
print("    box-sizing: border-box;")
print("    font-family: 'Courier New', Courier, monospace;")
print("   }")
print("   h2 {")
# print("    text-align: center;")
print("    margin: 1em;")
print("   }")
print("   body {")
print("    background-color: #f4f9f9;")
print("    display: flex;")
print("    flex-direction: column;")
print("    padding: 1rem 15%;")
print("   }")
print("   p {")
print("    margin: 0.5em;")
print("    word-wrap: break-word;")
print("   }")
print("  </style>")

print("</head>")
print("<body>")
print("<h2>Submitted post method Data:</h2>")
print("<p><strong>Name:</strong> " + str(name) + "</p>")
print("<p><strong>Email:</strong> " + str(email) + "</p>")
print("<p><strong>Message:</strong> " + str(message) + "</p>")