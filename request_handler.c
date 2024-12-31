#include "request_handler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *handle_get_request(HttpRequest request) {
    // Determine the size of the location buffer we need to allocate
    size_t location_size = strlen("sites/") + strlen(request.headers[0][1]) + strlen(request.path) + 1;  // +1 for the null terminator

    // Dynamically allocate memory for the location string
    char *location = malloc(location_size);

    if (location == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    strcpy(location, "sites/");
    strcat(location, request.headers[0][1]);
    strcat(location, request.path);

    return location;
}
