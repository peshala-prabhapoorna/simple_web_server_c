#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>

// run this when Ctrl + c is input from keyboard
void handler_sigint(int);

static bool keep_running = true;

const uint16_t PORT = 8000;

int main() {
    printf("staring server...\n");

    // handle SIGINT signal (Ctrl + c)
    struct sigaction act;
    act.sa_handler = handler_sigint;
    sigaction(SIGINT, &act, NULL);

    // create an endpoint for communication (fd: file descriptor)
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // bind a name (in address) to a socket
    struct sockaddr_in address = {
        AF_INET,
        // port number in reversed byte order
        htons(PORT),
        0
    };
    printf("listening on http://127.0.0.1:%d (press Ctrl + C to quit)\n",  
            PORT);
    bind(socket_fd, (struct sockaddr *) &address, sizeof(address));

    // listen for connection on a socket
    listen(socket_fd, 10);

    while (keep_running) {
        // accept a connection on a socket
        int client_socket_fd = accept(socket_fd, 0, 0);

        if (keep_running == false) {break;}

        // receive a message from socket (string from client)
        char buffer[256] = {0};
        recv(client_socket_fd, buffer, 256, 0);

        // GET /index.html ... 
        // extract 'index.html' from client request
        char* requested_file = buffer + 5;
        char* space_ptr = strchr(requested_file, ' ');
        if (space_ptr == NULL) {break;}
        *space_ptr = 0;

        // open requested file
        int opened_requested_file_fd = open(requested_file, O_RDONLY);

        // transfer data between file descriptors
        struct stat stat_buffer;
        stat(requested_file, &stat_buffer);
        sendfile(client_socket_fd, opened_requested_file_fd, 0, 
                stat_buffer.st_size);

        printf("127.0.0.1:%d \"%s\" 200 OK\n", address.sin_port, buffer);

        // close fd
        close(opened_requested_file_fd);
        close(client_socket_fd);
    }

    close(socket_fd);
    printf("server shutdown complete");

    return 0;
}

void handler_sigint(int) {
    keep_running = false;
    printf("\nshutting down server...\n"); 
}
