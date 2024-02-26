#!/usr/bin/env python
import cgi
import os

form = cgi.FieldStorage()


if 'uploadedfile' in form:
    upload = form['uploadedfile']
    if upload.filename:
        file_path = os.path.join('./upload', upload.filename)
        with open(file_path, 'wb') as file:
            file.write(upload.file.read())
        print("<h2>Upload Successfully</h2>")
        # Provide a download link for the uploaded PD
    else:
        print("<h2>No PDF selected.</h2>")
else:
    print("<h2>PDF upload failed.</h2>")