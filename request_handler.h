#include "common.h"

char *handle_get_request(HttpRequest request, int socket);
void send_response_header(int client_socket, int status_code, const char *status_message, const char *content_type, long content_length);
void send_file(int client_socket, const char *file_path);
void send_404(int client_socket);
long get_file_size(const char *file_path);  // Declaration of the function
