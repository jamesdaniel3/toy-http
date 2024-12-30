typedef struct {
    char method[16];    // HTTP Method (e.g., GET, POST)
    char path[256];     // Path (e.g., /index.html)
    char version[16];   // HTTP Version (e.g., HTTP/1.1)
    char headers[10][2][256]; // Example: Store headers as key-value pairs (e.g., [Host][localhost:8080])
} HttpRequest;

HttpRequest parse_request(const char *request);