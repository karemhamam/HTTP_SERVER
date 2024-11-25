/* *******************************************************
   * @File                 : main.c
   * @Author               : mohamed bahget hamam
   * @gmail                : mahamedhamam15@gmail.com
   *******************************************************
*/
/******************* include section start ****************** */

#include "http.h"

/******************* include section end ****************** */

/******************* main section start ****************** */

int main ()
{
    int server_socket , client_socket;
    struct sockaddr_in server_addr , client_addr ;
    socklen_t client_len = sizeof(client_addr);

    // create a socket
    server_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (server_socket < 0)
    {
        perror("Socket Creation Failed );");
        exit(1);
    }

    // Setup the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all available network interfaces
    server_addr.sin_port = htons(PORT);  // Set the server port

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) < 0) 
    {
        perror("Listen failed");
        close(server_socket);
        exit(1);
    }
    printf("Server is listening on port %d\n", PORT);

    // Main loop to accept client connections
    while (1) 
    {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0)
        {
            perror("Accept failed");
            continue;  // Continue accepting other clients
        }

        // Dynamically allocate memory for the client socket
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_socket;

        // Create a new thread to handle the client request
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, thread_function, new_sock);
        
        // Detach the thread so it cleans up automatically when finished
        pthread_detach(thread_id);
    }

    // Close the server socket after server shutdown (this line is never reached)
    close(server_socket);
    return 0;
}


/******************* main section start ****************** */

/* *******************************************************
   @User                 
   *******************************************************
   mohamed hamam         
*/