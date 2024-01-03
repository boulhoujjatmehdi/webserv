#!/usr/bin/env python3
import os
import html


print("<html><body>")

query_string = os.environ.get("QUERY_STRING", None)
if query_string is None:
    print("<p>Error! Error in passing data from form to script.</p>")
else:
    print("<p>The query string was: %s</p>" % html.escape(query_string))

print("</body></html>")