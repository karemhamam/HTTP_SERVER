#ifndef HTTP_H
#define HTTP_H

/* *******************************************************
   * @File                 : http.h
   * @Author               : mohamed bahget hamam
   * @gmail                : mahamedhamam15@gmail.com
   *******************************************************
*/

#include <stdio.h>      // for standard I/O functions
#include <stdlib.h>     // for memory management 
#include <string.h>     // for string operations 
#include <unistd.h>     // for POSIX API functions
#include <fcntl.h>      // for file control options
#include <pthread.h>    // for creating and managing threads
#include <sys/socket.h> // for socket programming functions
#include <netinet/in.h> // for network strctures
#include <sys/stat.h>   // for file and directory status
#include <dirent.h>     // for directory handling functions
#include <sys/wait.h>   // for wait system call


// the sever will listen on port 8080 for incoming HTTP requests
#define PORT 8080


/**
 * 
 * @brief : Handles an incoming HTTP request form a client
 * 
 * This function processes the client's HTTP request by reading the request data, 
 * parsing the HTTP method, requested path, and protocol version. Based on 
 * the request, the function decides whether to serve a directory listing, a file 
 * content, execute a CGI script, or send an error response.
 * 
 * if the HTTP method is not GET, the function sends a "405 method not allowed"
 * error. if the requested resource is a directory, it lists its contents.
 * if the resource is a regular file, it sends the file contents or excutes
 * the file as a CGI script if it has the .cgi extension. if the resource 
 * doesn't exist, a "404 not found" error is sent, as if the file not accessible
 * a "403 forbidden" error is returned.
 * 
 * @param clien_socket : the socket file descriptor for the client connection.
 */
void handle_client(int client_socket);


/**
 * @brief : sends an HTML dirctory listing response to the client.
 * 
 * this function generates an HTML page containing the list of files and directories 
 * in the specified directory and sends it as the response to the client.
 * the response is ssnt as a valid  HTTP response with a 200 ok status code.
 * the function will list the names of all files and directories inside the given path.
 * 
 * @param client_socket : the socket file descriptor for the client connection
 * @param path          : the path of the dirctory whose contents are to be listed.
 * 
 * @return void
 * 
 * @note the function assumes that the provided path is a valid directory path.
 * it doesn't handle cases where the path is invalid or isn't a directory.
 * it doesn't check for access restrictions either.  ): 
 */
void send_directory_listing(int client_socket , const char *path );


/**
 * @brief Sends the contents of a file to the client.
 * 
 * This function opent the specfied file, reads its contents, and sends them 
 * as the  HTTP response to the client. the response is sent with a "200 ok"
 * status and the "content-type" is determined based on the file's contenets
 * (text,HTML). the function will sent the entire file's contents
 * in chunks if the file is large.
 * 
 * @param client_socket the socket file descriptor for the client connection
 * @param path          the path of the file to send.
 * 
 * @return void
 * 
 * @note the function assumes that the provided path corresponds to a valid
 * regular file and doesn't perform checks for file existence or type.
 */
void send_file_contents(int client_socket , const char *path);


/**
 * @brief Sends an HTTP error response to the client
 * 
 * This function sends an HTTP response with an error status code
 * like "404" for Not Found, 500 for internal server error.
 * the response includes a standard HTTP header and a simple HTML page withe an error message
 * explaining the issue. The response body contains the error message as part ot HTML content.
 * 
 * @param client_socket  the socket file descriptor for the client connection
 * @param status_code    the HTTP status code to send
 * @param message        a brief description or message related to the error.
 * 
 * @return void
 * 
 * @note the function assumes that the provided status_code corresponds to a 
 * valid HTTP status code and that the message is a string suitable for
 * displaying in an HTML error page.    
 */
void send_error_response(int client_socket , int status_code , const char *message);


/**
 * @brief Executes a CGI script and sends the output to the client.
 * 
 * This functin executes a CGI script located
 * at the specified path. the script is executed using the fork() and ecex()
 * system calls , allowing the server to spawn a new process to handle the execution of 
 * the script. the standard output of the GCI is sent to the client as part of the 
 * HTTP RESPONSE.
 * 
 * @param client_socket the socket file descriptor for the client connection
 * @param path the path to the CGI script to execute.
 * 
 * @return void
 * 
 * @note the function assumes that the provided path corresponds to a valid GCI script
 * the server should ensure that the CGI script is executable 
 * and properly configured to run in the server environment. 
 */
void execute_gci_script(int client_socket , const char *path);

/**
 * @brief The function executed by each thread to handle client requests.
 * 
 * this function is called in a new thread to handle each incoming client connection.
 * it receives the client's socket file descroptor as an argument, processes the 
 * client's request by calling handle_client() and then closes the socket after
 * the request is handled.
 * 
 * @param arg the argument passed to the thread , which is a pinter to the client socket.
 * 
 * @return NULL 
 * 
 * @note the arg is expected to be a pointer to an integet containing the 
 * client socket file descriptor, it is freed after use.
 */
void *thread_function(void *arg);


/* *******************************************************
   @User                 
   *******************************************************
   mohamed hamam         
*/

#endif
