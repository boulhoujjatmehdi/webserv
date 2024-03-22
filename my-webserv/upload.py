#!/usr/bin/env python
import cgi
import os

form = cgi.FieldStorage()

if 'uploadedfile' in form:
    upload = form['uploadedfile']
    if upload.filename:
        file_path = os.path.join('./upload', upload.filename)
        if not os.path.exists(os.path.dirname(file_path)):
            os.makedirs(os.path.dirname(file_path))
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