#!/bin/bash

# Output the content-type header
echo "Content-Type: text/html"
echo

# Output the HTML content
echo "<html>"
echo "<head><title>CGI Script Example</title></head>"
echo "<body>"
echo "<h1>Hello from CGI!</h1>"
echo "<p>Current Date and Time: $(date)</p>"
echo "</body>"
echo "</html>"

