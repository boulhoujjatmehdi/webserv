#!/usr/bin/env python
import cgi
import os

form = cgi.FieldStorage()

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
        print("<h2>Nothing selected.</h2>")
else:
    print("<h2>upload failed.</h2>")