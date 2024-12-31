#include <string.h>
#include <stdio.h>

#include "http_parser.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))

HttpRequest parse_request(const char *request) {
    HttpRequest parsed_request;
    memset(&parsed_request, 0, sizeof(HttpRequest));

    char buffer[1024];
    char first_line_buffer[1024];
    strncpy(buffer, request, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    strncpy(first_line_buffer, buffer, sizeof(first_line_buffer) - 1);
    first_line_buffer[sizeof(first_line_buffer) - 1] = '\0';

    // Parse first line
    char *line = strtok(first_line_buffer, "\r\n");
    if (line == NULL) {
        return parsed_request;
    }

    char *method = strtok(line, " ");
    char *path = strtok(NULL, " ");
    char *version = strtok(NULL, " \r\n");

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
    line = strtok(buffer, "\r\n");  // Skip first line
    while ((line = strtok(NULL, "\r\n")) != NULL && header_index < 10) {
        printf("Header line: '%s'\n", line);
        if (strlen(line) == 0) {
            continue;
        }

        // Instead of using strtok, use strchr to find the colon
        char *colon = strchr(line, ':');
        if (colon != NULL) {
            size_t key_length = colon - line;
            char *value = colon + 1;

            // Skip leading whitespace in value
            while (*value == ' ') value++;

            // Copy key (everything before the colon)
            strncpy(parsed_request.headers[header_index][0], line, 
                    MIN(key_length, sizeof(parsed_request.headers[header_index][0]) - 1));
            parsed_request.headers[header_index][0][MIN(key_length, sizeof(parsed_request.headers[header_index][0]) - 1)] = '\0';
            
            // Copy value (everything after the colon and space)
            strncpy(parsed_request.headers[header_index][1], value,
                    sizeof(parsed_request.headers[header_index][1]) - 1);
            parsed_request.headers[header_index][1][sizeof(parsed_request.headers[header_index][1]) - 1] = '\0';
            
            header_index++;
        }
    }

    return parsed_request;
}