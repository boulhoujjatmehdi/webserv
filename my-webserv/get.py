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
print("<h2>Submitted Form Data:</h2>")
print("<p><strong>Name:</strong> " + str(name) + "</p>")
print("<p><strong>Email:</strong> " + str(email) + "</p>")
print("<p><strong>Message:</strong> " + str(message) + "</p>")


if 'uploadedfile' in form:
    upload = form['uploadedfile']
    if upload.filename:
        file_path = os.path.join('./upload', upload.filename)
        with open(file_path, 'wb') as file:
            chunk = upload.file.read(1024)
            while chunk:
                file.write(chunk)
                chunk = upload.file.read(1024)
        print("<h2>Upload Successfully</h2>")
    else:
        print("<h2>No PDF selected.</h2>")
else:
    print("<h2>PDF upload failed.</h2>")