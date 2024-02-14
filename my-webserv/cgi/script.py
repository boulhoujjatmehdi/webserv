#!/usr/bin/env python3
import os
import urllib.parse

print("Content-Type: text/plain")
print()

query_string = os.getenv("QUERY_STRING", "")
params = urllib.parse.parse_qs(query_string)

for key, values in params.items():
    for value in values:
        print(f"{key}: {value}")