#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int address_len = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        // The socket method creates a new socket with TYPE, DOMAIN, and PROTOCOL given
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

    // Setup the server address, setting address family to AF_INET (IPv4)
    address.sin_family = AF_INET;

    address.sin_addr.s_addr = INADDR_ANY;

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

    printf("Connection accepted");

    // Read data from the client and print it 
    ssize_t valread;

    while((valread = read(new_socket, buffer, BUFFER_SIZE)) > 0){
        printf("Client: %s", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the socket 
    close(server_fd);
    return 0;
}