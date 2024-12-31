#include "request_handler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>  

#define BUFFER_SIZE 1024

char *handle_get_request(HttpRequest request, int socket) {
    // Determine the size of the location buffer we need to allocate
    size_t location_size = strlen("sites/") + strlen(request.headers[0][1]) + strlen(request.path) + 1;  // +1 for the null terminator

    // Dynamically allocate memory for the location string
    char *file_location = malloc(location_size);

    if (file_location == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    strcpy(file_location, "sites/");
    strcat(file_location, request.headers[0][1]);
    strcat(file_location, request.path);

    // Try to serve the file
    FILE *file = fopen(file_location, "rb");
    if (file) {
        // File exists, send HTTP headers and file content
        send_response_header(socket, 200, "OK", "text/html", get_file_size(file_location));
        send_file(socket, file_location);
        fclose(file);
    } else {
        // File not found, send 404 error
        send_404(socket);
    }

    // Free the dynamically allocated memory for file location
    free(file_location);

    return file_location;
}

// Function to send an HTTP response header
void send_response_header(int client_socket, int status_code, const char *status_message, const char *content_type, long content_length) {
    char response_header[BUFFER_SIZE];
    
    // Format the response header
    snprintf(response_header, sizeof(response_header),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Connection: close\r\n"
             "\r\n", status_code, status_message, content_type, content_length);
    
    // Send the response header
    send(client_socket, response_header, strlen(response_header), 0);
}

// Function to send the file content
void send_file(int client_socket, const char *file_path) {
    FILE *file = fopen(file_path, "rb");  // Open the file in binary mode
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);  // Reset file pointer to the beginning

    // Send the file content in chunks
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(file);  // Close the file
}

// Function to send a 404 error page
void send_404(int client_socket) {
    const char *not_found = "<html><body><h1>404 Not Found</h1></body></html>";
    send_response_header(client_socket, 404, "Not Found", "text/html", strlen(not_found));
    send(client_socket, not_found, strlen(not_found), 0);
}

// Function to get the size of a file
long get_file_size(const char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        return -1;  // If the file cannot be opened, return an error code
    }
    
    fseek(file, 0, SEEK_END);  // Move to the end of the file
    long size = ftell(file);   // Get the file size
    fclose(file);              // Close the file
    return size;               // Return the file size
}