#include <string.h>
#include <stdio.h>

#include "http_parser.h"

HttpRequest parse_request(const char *request) {
    HttpRequest parsed_request;

    char buffer[1024];
    strncpy(buffer, request, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char *line = strtok(buffer, "\n");
    
    // take the method, path, and version from line 1
    int location = 0;
    char *sub_line = strtok(line, " ");

    while(sub_line != NULL) {
        sub_line = strtok(NULL, " ");
        location++;
        switch (location)
        {
        case 1:
            strncpy(parsed_request.method, sub_line, sizeof(parsed_request.method) - 1);
            parsed_request.method[sizeof(parsed_request.method) - 1] = '\0';
            break;
        case 2:
            strncpy(parsed_request.path, sub_line, sizeof(parsed_request.path) - 1);
            parsed_request.path[sizeof(parsed_request.path) - 1] = '\0';
            break;
        case 3:
            strncpy(parsed_request.version, sub_line, sizeof(parsed_request.version) - 1);
            parsed_request.version[sizeof(parsed_request.version) - 1] = '\0';
            break;
        default:
            break;
        }
    }

    // Split the rest of the lines into key-value pairs to be stored in headers
    int header_index = 0; // Keep track of the current header index
    while ((line = strtok(NULL, "\n")) != NULL) {
        // Split line on ": "
        char *sub_key = strtok(line, ": ");
        char *sub_val = strtok(NULL, "");

        // Ensure both key and value are non-NULL
        if (sub_key != NULL && sub_val != NULL && header_index < 10) {
            // Copy key into headers[header_index][0]
            strncpy(parsed_request.headers[header_index][0], sub_key, sizeof(parsed_request.headers[header_index][0]) - 1);
            parsed_request.headers[header_index][0][sizeof(parsed_request.headers[header_index][0]) - 1] = '\0'; // Null-terminate

            // Copy value into headers[header_index][1]
            strncpy(parsed_request.headers[header_index][1], sub_val, sizeof(parsed_request.headers[header_index][1]) - 1);
            parsed_request.headers[header_index][1][sizeof(parsed_request.headers[header_index][1]) - 1] = '\0'; // Null-terminate

            header_index++; // Move to the next header slot
        }
    }


    return parsed_request;

}