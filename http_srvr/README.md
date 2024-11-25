
# HTTP Server in C

## Description

This project implements a basic HTTP server in C. The server supports the following features:
- Accepts connections from web browsers.
- Reads and parses HTTP requests.
- Processes requests based on the type of resource:
  1. **Directories**: Lists the contents of the directory.
  2. **Regular Files**: Sends the contents of the file to the client.
  3. **CGI Scripts**: Executes the script and returns its output.
  4. **Nonexistent Resources**: Responds with an appropriate HTTP error message.
- Sends appropriate HTTP responses to clients.
- Supports concurrent access (bonus feature).

This server adheres to HTTP/1.1 standards and is built using POSIX socket APIs. It can handle multiple clients simultaneously using threads.

---

## Features

- **HTTP Methods Supported**:
  - `GET`: Retrieve resources (directories, files, or CGI outputs).
- **Dynamic Content Execution**:
  - Executes CGI scripts and returns their output.
- **Error Handling**:
  - Responds with `404 Not Found Ya Bash );` for invalid requests.
- **Concurrency**:
  - Handles multiple client connections using threads.

---

## Directory Structure

```
http_server/
├── http_srvr/
│   ├── http.h                 # Header file containing declarations
│   ├── http.c                 # Core HTTP processing logic
│   ├── main.c                 # Entry point for the server
│
├── cgi-bin/
│   ├── example.cgi            # Example CGI script
|
├── README.md                  # Project documentation
```

---

## Requirements

- **Compiler**: GCC (or any C99-compliant compiler)
- **Operating System**: Linux or any POSIX-compatible system
- **Tools**:
  - `curl` or a web browser for testing
  - `nano` or any text editor to create test files/scripts

---

## Installation and Setup

### Step 1: Clone the Repository
```bash
git clone <repository_url>
cd http_server
```

### Step 2: Compile the Server
Navigate to the `http_srvr/` directory:
```bash
cd http_srvr
gcc -o http_server *.c
```
This generates an executable named `http_server`.

### Step 3: Create a Directory for Testing
Create test directories and files for testing:
```bash
mkdir -p test_directory
echo "This is a test file" > test_directory/test_file.txt
```

### Step 4: Create a CGI Script
Navigate to the `cgi-bin/` directory and create a CGI script:
```bash
cd ../cgi-bin
nano example.cgi
```
Example content for `example.cgi`:
```bash
#!/bin/bash
echo "Content-Type: text/html"
echo ""
echo "<html><body><h1>CGI Script Output</h1></body></html>"
```
Make the script executable:
```bash
chmod +x example.cgi
```

---

## Usage

### Start the Server
Run the server from the `http_srvr/` directory:
```bash
./http_server
```

### Test with a Web Browser
Visit the following URLs in your browser:
- **List Directory Contents**:  
  `http://localhost:8080/test_directory/`
- **View a File**:  
  `http://localhost:8080/test_directory/test_file.txt`
- **Run a CGI Script**:  
  `http://localhost:8080/cgi-bin/example.cgi`
- **Handle Errors**:  
  Test a nonexistent resource, e.g., `http://localhost:8080/does_not_exist`

### Test with `curl`
You can also use `curl` to test server responses:
```bash
curl http://localhost:8080/test_directory/
curl http://localhost:8080/test_directory/test_file.txt
curl http://localhost:8080/cgi-bin/example.cgi
curl http://localhost:8080/nonexistent
```

---

## Features to Test

### Basic Functionality
1. **File Listing**: Ensure directories are listed correctly.
2. **File Serving**: Verify that text files are sent properly.
3. **CGI Execution**: Confirm CGI scripts execute and return their output.
4. **Error Handling**: Test with nonexistent paths to ensure proper error responses.

### Concurrent Access
1. Open multiple browser tabs or use `curl` concurrently to test server performance.

---

## Troubleshooting

1. **Port Binding Error**:
   - If the port is already in use, change the `PORT` in `http.h` or kill the process using the port.
   - Find the process:
     ```bash
     sudo netstat -tuln | grep 8080
     ```
     Kill the process:
     ```bash
     sudo kill <pid>
     ```

2. **Permission Denied**:
   - Ensure the CGI script is executable:
     ```bash
     chmod +x example.cgi
     ```

3. **Missing Dependencies**:
   - Ensure the necessary tools are installed:
     ```bash
     sudo apt install gcc make curl
     ```

---

## Documentation

This project is fully documented using Doxygen(working on it....). To generate the documentation:
1. Install Doxygen:
   ```bash
   sudo apt install doxygen
   ```
2. Generate the documentation:
   ```bash
   doxygen Doxyfile
   ```
3. Open the documentation:
   Navigate to `docs/html/index.html` in your browser.

---

## License

This project is licensed under mohamadhama@gmail.com.

