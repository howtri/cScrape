// Main file for the web server web scraper.

#include "web_scraper_server.h"
#include "web_scraper_queue.h"
#include "web_scraper_handlers.h"

#define ROUTE_SCRAPE '1'
#define ROUTE_RETURN '2'

// Create an IPv4 socket address.
struct sockaddr_in
initialize_server_address ()
{
    // Create a IPV4 address that can accept any IP addresses as connections.
    struct sockaddr_in address = { 0 };
    address.sin_family         = AF_INET;
    address.sin_addr.s_addr    = INADDR_ANY;
    // Listen on our specified port.
    address.sin_port = htons(PORT);
    return address;
}

// Creates a socket, server_address, and begins listening on the socket.
int
start_server (int max_connections)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == socket_fd)
    {
        perror("Failed to create socket");
        return -1;
    }

    struct sockaddr_in address = initialize_server_address();

    // Casts from an IPv4 address to a generic address, IPv4 address is compatible with generic address.
    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        close(socket_fd);
        perror("Failed to bind to the address");
        return -1;
    }

    // Passively accept connections.
    if (listen(socket_fd, max_connections) == -1)
    {
        close(socket_fd);
        perror("Failed to listen on the socket fd");
        return -1;
    }

    return socket_fd;
}

int
accept_new_connection (int socket_fd)
{
    // Create a new socket for our connection.
    struct sockaddr_in address     = { 0 };
    int                address_len = sizeof(address);
    // The casts below are safe as struct sockaddr and struct sockaddr_in are
    // designed to be interoperable. int to socklen_t is safe when socklen_t is
    // not smaller than the size of int on the system it will run on.
    int connection_socket = accept(
        socket_fd, (struct sockaddr *)&address, (socklen_t *)&address_len);
    if (connection_socket < 0)
    {
        perror("Failed to accept connection.");
    }
    return connection_socket;
}

void accept_new_connections(int listening_socket, struct pollfd p_fds[], int max_connections) {
    int new_socket = accept_new_connection(listening_socket);
    if (new_socket < 0) {
        fprintf(stderr, "Failed to accept new connection.\n");
        return;
    }

    for (int i = 1; i < max_connections; ++i) {
        // Slot is empty/unused when -1.
        if (p_fds[i].fd == -1) {
            p_fds[i].fd = new_socket;
            p_fds[i].events = POLLIN;
            return;
        }
    }

    fprintf(stderr, "No available slots for new connections.\n");
    close(new_socket);
}

ssize_t
receive_message (int connection_socket, char * p_buffer, size_t buffer_size)
{
    // Retain space for a null-byte terminator.
    ssize_t bytes_read = read(connection_socket, p_buffer, buffer_size - 1);
    printf("Bytes read %ld.\n", bytes_read);
    if (-1 == bytes_read)
    {
        perror("Failed to read from socket");
        return -1;
    }
    // Null terminate the buffer
    p_buffer[bytes_read] = '\0';
    return bytes_read;
}

void process_existing_connections(struct pollfd p_fds[], int max_connections, int buffer_size, queue_t * p_url_queue) {
    for (int i = 1; i < max_connections; ++i) {
        if (!(p_fds[i].revents & POLLIN)) {
            continue; // Skip if no data to read
        }

        char buffer[buffer_size];
        memset(buffer, 0, buffer_size); // Initialize buffer to ensure it's null-terminated
        ssize_t bytes_read = receive_message(p_fds[i].fd, buffer, buffer_size - 1);
        
        if (bytes_read <= 0) {
            close(p_fds[i].fd);
            p_fds[i].fd = -1; // Mark as available.
            continue;
        }

        buffer[bytes_read] = '\0'; // Ensure buffer is null-terminated
        printf("Received message: %s\n", buffer);

        // Parse the message
        char* action = strtok(buffer, " ");
        char* url = strtok(NULL, "");

        if ((!action) || (!url) || (strlen(action) != 1)) {
            fprintf(stderr, "Error: Incorrect message format. Expected format is <one digit number> <url>\n");
            continue; // Move to the next connection if format is incorrect
        }

        switch (action[0]) {
            case ROUTE_SCRAPE:
                if (handle_scrape_new_request(url, p_url_queue) == EXIT_FAILURE) {
                    fprintf(stderr, "Failed to enqueue URL for scraping.\n");
                }
                break;
            case ROUTE_RETURN:
                handle_return_scrape_request();
                break;
            default:
                fprintf(stderr, "Error: Unrecognized action '%c'. Expected options are 1 or 2.\n", action[0]);
                break;
        }
    }
}

// TODO: Exit on Signal or some other condition.
void handling_loop(int listening_socket, int max_connections, queue_t * p_url_queue) {
    struct pollfd * p_fds = malloc(max_connections * sizeof(struct pollfd));
    if (p_fds == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    // Initialize all to -1. On some platforms fd can be larger than a byte so its safest to loop over and set to -1.
    for (int fd_iter = 0; fd_iter < max_connections; fd_iter++)
    {
        p_fds[fd_iter].fd = -1;
    }
    p_fds[0].fd = listening_socket;
    p_fds[0].events = POLLIN;

    const int buffer_size = 1024; // Define appropriately.

    int count_run = 0;
    while (true) {
        if (poll(p_fds, max_connections, -1) == -1) {
            perror("Poll error");
            free(p_fds);
            return;
        }

        if (p_fds[0].revents & POLLIN) {
            accept_new_connections(listening_socket, p_fds, max_connections);
        }

        process_existing_connections(p_fds, max_connections, buffer_size, p_url_queue);
        if (count_run > 2)
        {
            printf("Breaking from the loop for testing purposes.\n");
            break;
        }
        count_run++;
        printf("Count run is %d.\n", count_run);
    }

    free(p_fds);
}

int
cleanup_server (int socket)
{
    close(socket);
    return 0;
}

// end of file.
