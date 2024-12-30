#include <string.h>
#include <stdio.h>

#include "http_parser.h"

HttpRequest parse_request(const char *request) {
    printf("Client: %s", request);

}