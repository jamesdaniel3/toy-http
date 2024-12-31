#include <string.h>
#include <stdio.h>

#include "http_parser.h"

HttpRequest parse_request(const char *request) {
    HttpRequest parsed_request;
    memset(&parsed_request, 0, sizeof(HttpRequest));  // Initialize all fields to zero

    char buffer[1024];
    strncpy(buffer, request, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // Get the first line
    char *line = strtok(buffer, "\r\n");
    if (line == NULL) {
        return parsed_request;
    }

    // Parse first line (METHOD PATH VERSION)
    char *method = strtok(line, " ");
    char *path = strtok(NULL, " ");
    char *version = strtok(NULL, " \r\n");

    // Safely copy if tokens exist
    if (method) {
        strncpy(parsed_request.method, method, sizeof(parsed_request.method) - 1);
        parsed_request.method[sizeof(parsed_request.method) - 1] = '\0';
    }
    if (path) {
        strncpy(parsed_request.path, path, sizeof(parsed_request.path) - 1);
        parsed_request.path[sizeof(parsed_request.path) - 1] = '\0';
    }
    if (version) {
        strncpy(parsed_request.version, version, sizeof(parsed_request.version) - 1);
        parsed_request.version[sizeof(parsed_request.version) - 1] = '\0';
    }

    // Parse headers
    int header_index = 0;
    while ((line = strtok(NULL, "\r\n")) != NULL && header_index < 10) {
        // Skip empty lines
        if (strlen(line) == 0) {
            continue;
        }

        char *key = strtok(line, ":");
        char *value = strtok(NULL, "\r\n");

        if (key && value) {
            // Skip leading whitespace in value
            while (*value == ' ') value++;
            
            strncpy(parsed_request.headers[header_index][0], key, sizeof(parsed_request.headers[header_index][0]) - 1);
            parsed_request.headers[header_index][0][sizeof(parsed_request.headers[header_index][0]) - 1] = '\0';
            
            strncpy(parsed_request.headers[header_index][1], value, sizeof(parsed_request.headers[header_index][1]) - 1);
            parsed_request.headers[header_index][1][sizeof(parsed_request.headers[header_index][1]) - 1] = '\0';
            
            header_index++;
        }
    }

    return parsed_request;
}