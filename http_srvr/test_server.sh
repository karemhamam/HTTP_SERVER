#!/bin/bash

# Test valid file
curl -s http://localhost:8080/test_file.txt > /dev/null && echo "File Test: Passed" || echo "File Test: Failed"

# Test directory listing
curl -s http://localhost:8080/test_dir/ > /dev/null && echo "Directory Test: Passed" || echo "Directory Test: Failed"

# Test CGI script
curl -s http://localhost:8080/test_script.cgi > /dev/null && echo "CGI Test: Passed" || echo "CGI Test: Failed"

# Add more test cases as needed...

