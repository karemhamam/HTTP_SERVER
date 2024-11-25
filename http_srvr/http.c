/* *******************************************************
   * @File                 : http.c
   * @Author               : mohamed bahget hamam
   * @gmail                : mahamedhamam15@gmail.com
   *******************************************************
*/

/******************* include section start ****************** */

#include "http.h"

/******************* include section end   ****************** */


/******************* defination section start **************** */

void handle_client(int client_socket)
{
    // buffer to hold the client's HTTP request
    char buffer[1024], method[10], path[512], protocol[10];

    // read the request from the client socket
    read(client_socket, buffer ,sizeof(buffer) - 1);

    // parse the HTTP request into method, path and protocol
    sscanf(buffer,"%s %s %s",method, path, protocol);

    // if the HTTP method is not GET , return 405 Method Not Allowed a Basha );
    if (strcmp(method,"GET") != 0 )
    {
        send_error_response(client_socket, 405 , "Method Not Allowed Ya Basha );");
        return;
    }

    // remove leading slash from the path
    if (path[0] == '/')
    {
        memmove(path, path + 1 , strlen(path));
    }

    // check the file status (whether it's a file , directory or doesn't exit)
    struct stat st;
    if (stat(path, &st) < 0)
    {
        // if the file doesn't exit, sent 404 Not Found a Basha );
        send_error_response(client_socket, 404 , "Not Found Ya Basha );");
    }else if (S_ISDIR(st.st_mode))
    {   
        // if it's a directory , send a dirctory listing
        send_directory_listing(client_socket,path);
    }else if (S_ISREG(st.st_mode))
    {
        // if it's a regular file, check if it's a CGI script
        if (strstr(path , ".gci"))
        {
            execute_gci_script(client_socket , path);
        }else
        {
            // if it's regular file , send the file contents
            send_file_contents(client_socket, path);
        }
        
    }else
    {
        // if it's not a file or directory, send 403 Forbidden a Basha );
        send_error_response(client_socket , 403 , "Forbidden Ya Basha );");
    }
}


void send_directory_listing(int client_socket, const char *path) 
{
    DIR *dir = opendir(path);
    if (!dir) {
        send_error_response(client_socket, 500, "Failed to open directory");
        return;
    }

    // Send HTTP header
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), 
             "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    send(client_socket, buffer, strlen(buffer), 0);

    // Send directory listing in HTML format
    snprintf(buffer, sizeof(buffer), 
             "<html><body><h1>Directory Listing for %s</h1><ul>", path);
    send(client_socket, buffer, strlen(buffer), 0);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        snprintf(buffer, sizeof(buffer), 
                 "<li><a href=\"%s/%s\">%s</a></li>", path, entry->d_name, entry->d_name);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    snprintf(buffer, sizeof(buffer), "</ul></body></html>");
    send(client_socket, buffer, strlen(buffer), 0);

    closedir(dir);
}



void send_file_contents(int client_socket , const char *path)
{
    // open the file for reading in binary mode 
    FILE *file = fopen(path, "rb");

    // if the file could not be opened , return a 404 error
    if (file == NULL)
    {
        send_error_response(client_socket, 404 , "Not Found Ya Basha );");
        return;
    }

    // get the file extension to set the appropriate content type
    const char *content_type = "application/octet-stream"; // defualt to binary
    if (strstr(path , ".html"))
    {
        content_type = "text/html";
    }else if (strstr(path , ".txt"))
    {
        content_type = "text/plian";
    }else if (strstr(path ,".css"))
    {
        content_type = "text/css";
    }else if (strstr(path , ".js"))
    {
        content_type = "application/javascript";
    }
    else
    {};

    // send HTTP 200 OK header and cotent type
    char response_header[256];
    snprintf(response_header , sizeof(response_header) , "HTTP/1.1 200 ok\r\nContent-Type: %s\r\n\r\n" , content_type);
    send(client_socket , response_header , strlen(response_header) , 0);

    // read the file in chunks and sent it to the client
    char buffer[1024];
    size_t bytes_read;
    while((bytes_read = fread(buffer , 1 , sizeof(buffer) , file)) > 0)
    {
        send(client_socket , buffer , bytes_read , 0 );
    }

    // close the file after sending its contents
    fclose(file);
}


void send_error_response(int client_socket , int status_code , const char *message)
{
    // HTTP response header 
    char response_header[256];

    // format the status code and message into the response header
    snprintf(response_header , sizeof(response_header) , "HTTP/1.1 %d %s\r\nContent-Type: text/html\r\n\r\n" , status_code , message);

    //send the header to the client
    send(client_socket , response_header , strlen(response_header) , 0);

    // HTML content body with the error message
    char response_body[256];
    snprintf(response_body , sizeof(response_body) , "<html><body><h1>%d %s</h1><p>%s</p></body></html>" , status_code , message , message);

    // send the error message content to the client 
    send(client_socket , response_body , strlen(response_body) , 0);
}

void execute_gci_script(int client_socket , const char *path)
{
    // fork the process to handle the CGI script execution
    pid_t pid = fork();
    
    if (pid < 0)
    {
        // if fork() fails, send an internal server error
        send_error_response(client_socket , 500 , "Inernal Server Error Ya Basha );");
        return;
    }
    
    if (pid == 0)
    {
        // child process: execute the CGI script
        // redirect stdout to the cliend socket
        dup2(client_socket , STDOUT_FILENO);

        // execute the CGI script using execvp (this replaces the process with the script)
        execlp(path ,path ,(char *)NULL);

        // if execvp fails send an internal server error
        send_error_response(client_socket , 500 , "Internal Server Error Ya Basha );");
        // exit the child process after execution failure
        exit(0);
    }else
    {
        // parent process wait for the child process to finish
        wait(NULL);
    }
}


void *thread_function(void *arg)
{
    int client_socket = *(int *)arg;
    
    // free the memory used to store the socket pointer
    free(arg); 

    // handle the client's request 
    handle_client(client_socket);
    
    // close the client socket after handling
    close(client_socket);

    return NULL;
}
/******************* defination section end     ***************** */


/* *******************************************************
   @User                 
   *******************************************************
   mohamed hamam         
*/
