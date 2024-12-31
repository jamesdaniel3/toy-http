#ifndef COMMON_H   // Check if COMMON_H is not already defined
#define COMMON_H   // Define COMMON_H to prevent multiple inclusions

typedef struct {
    char method[16];    // HTTP Method (e.g., GET, POST)
    char path[256];     // Path (e.g., /index.html)
    char version[16];   // HTTP Version (e.g., HTTP/1.1)
    char headers[10][2][256]; // Example: Store headers as key-value pairs (e.g., [Host][localhost:8080])
} HttpRequest;

#endif // COMMON_H   // End of include guard