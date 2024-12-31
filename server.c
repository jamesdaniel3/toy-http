#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "http_parser.h"

#define PORT 8080
#define BUFFER_SIZE 1024


// helper method to confirm that the HttpRequest is being created correctly
void print_http_request(const HttpRequest *request) {
    printf("\n=== HTTP Request Debug Output ===\n");
    
    // Print method, path, and version
    printf("Method: '%s'\n", request->method);
    printf("Path: '%s'\n", request->path);
    printf("Version: '%s'\n", request->version);

    // Print headers with more debug info
    printf("\nHeaders Debug:\n");
    for (int i = 0; i < 10; i++) {
        printf("Header[%d] - Key length: %zu, Value length: %zu\n", 
               i, 
               strlen(request->headers[i][0]), 
               strlen(request->headers[i][1]));
        
        if (strlen(request->headers[i][0]) > 0) {
            printf("  '%s': '%s'\n", 
                   request->headers[i][0], 
                   request->headers[i][1]);
        }
    }
    
    printf("\n=== End of Request ===\n");
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int address_len = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create a socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { 
        perror("Socket failed, exiting");
        exit(EXIT_FAILURE);
    }

    // Set socket options individually
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt for SO_REUSEADDR failed");
        exit(EXIT_FAILURE);
    }

    #ifdef SO_REUSEPORT // Check if SO_REUSEPORT is defined on your system
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt for SO_REUSEPORT failed");
        exit(EXIT_FAILURE);
    }
    #endif

    // Setup the server
    address.sin_family = AF_INET;

    address.sin_addr.s_addr = INADDR_ANY; // tells the server to accept connections on any of the server's available IP addresses

    address.sin_port = htons(PORT);

    // Check if binding the socket failed
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed, exiting");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listening failed, exiting");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept incoming connections
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&address_len);
    if (new_socket < 0) {
        perror("could not accept connection, exiting");
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted\n");

    // Read data from the client and print it 
    ssize_t valread;

    while((valread = read(new_socket, buffer, BUFFER_SIZE)) > 0){
        printf("'%s'", buffer);
        HttpRequest parsed_request = parse_request(buffer);
        print_http_request(&parsed_request);
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the socket 
    close(server_fd);
    return 0;
}