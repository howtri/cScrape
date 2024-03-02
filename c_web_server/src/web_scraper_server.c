// Contains logic for starting the web server and accepting connections using
// poll.

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "web_scraper_server.h"
#include "web_scraper_handlers.h"

#define PORT         8082
#define BUFFER_SIZE  1024
#define ROUTE_SCRAPE '1'
#define ROUTE_RETURN '2'

// Use to determine when the server should begin shutting down.
// Any URLs still being processed by threads will continue.
volatile sig_atomic_t shutdown_requested = 0;

// Modify the sigint_handler to set this flag
void
sigint_handler (int sig_num)
{
    shutdown_requested = 1;
}

// Create an IPv4 socket address.
static struct sockaddr_in
initialize_server_address ()
{
    // Create an IPV4 address that can accept any IP addresses as connections.
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

    // Casts from an IPv4 address to a generic address, IPv4 address is
    // compatible with generic address.
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

static int
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

static void
accept_new_connections (int           listening_socket,
                        struct pollfd p_fds[],
                        int           max_connections)
{
    int new_socket = accept_new_connection(listening_socket);
    if (new_socket < 0)
    {
        fprintf(stderr, "Failed to accept new connection.\n");
        return;
    }

    for (int i = 1; i < max_connections; ++i)
    {
        // Slot is empty/unused when -1.
        if (p_fds[i].fd == -1)
        {
            p_fds[i].fd     = new_socket;
            p_fds[i].events = POLLIN;
            return;
        }
    }

    fprintf(stderr, "No available slots for new connections.\n");
    close(new_socket);
}

static ssize_t
receive_message (int connection_socket, char *p_buffer, size_t buffer_size)
{
    // Retain space for a null-byte terminator.
    ssize_t bytes_read = read(connection_socket, p_buffer, buffer_size - 1);
    if (-1 == bytes_read)
    {
        perror("Failed to read from socket");
        return -1;
    }
    // Null terminate the buffer
    p_buffer[bytes_read] = '\0';
    return bytes_read;
}

static void
process_existing_connections (struct pollfd p_fds[],
                              int           max_connections,
                              int           buffer_size,
                              queue_t      *p_url_queue)
{
    for (int i = 1; i < max_connections; ++i)
    {
        if (!(p_fds[i].revents & POLLIN))
        {
            continue; // Skip if no data to read
        }

        char buffer[buffer_size];
        memset(buffer,
               0,
               buffer_size); // Initialize buffer to ensure its null-terminated
        ssize_t bytes_read
            = receive_message(p_fds[i].fd, buffer, buffer_size - 1);

        if (bytes_read <= 0)
        {
            close(p_fds[i].fd);
            p_fds[i].fd = -1; // Mark as available.
            continue;
        }

        buffer[bytes_read] = '\0';

        // Use strtok_r for thread-safe tokenization.
        char *saveptr;
        char *action = strtok_r(buffer, " ", &saveptr);
        char *url    = strtok_r(NULL, "", &saveptr);

        if ((!action) || (!url) || (strlen(action) != 1))
        {
            fprintf(stderr,
                    "Incorrect message format. Expected format is <one digit "
                    "number> <url>\n");
            handle_invalid_request(p_fds[i].fd);
            continue;
        }

        char safe_url[BUFFER_SIZE];
        strncpy(safe_url, url, buffer_size - 1);
        safe_url[buffer_size - 1] = '\0'; // Ensure null termination

        switch (action[0])
        {
            case ROUTE_SCRAPE:
                if (handle_scrape_new_request(
                        p_fds[i].fd, safe_url, p_url_queue)
                    == EXIT_FAILURE)
                {
                    fprintf(stderr, "Failed to enqueue URL for scraping.\n");
                }
                break;
            case ROUTE_RETURN:
                handle_return_scrape_request(p_fds[i].fd, safe_url);
                break;
            default:
                fprintf(stderr, "Option not allowed %c.\n", action[0]);
                handle_invalid_request(p_fds[i].fd);
                break;
        }
    }
}

// Continue accepting new connections from clients (up to the connection limit)
// and process already established connections in a loop utilizing poll.
// Existing connections are processed and completed using one of the three web
// scraper handlers. Web scraper handlers can pass tasks to the thread pool for
// asynchronous completion for web scraping. Web server begins shutting down on
// SIGINT.
void
handling_loop (int listening_socket, int max_connections, queue_t *p_url_queue)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask); // Initialize the signal set to empty
    sa.sa_flags = 0;          // No flags

    // Register the signal handler for SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("Error setting up signal handler");
        exit(EXIT_FAILURE);
    }

    struct pollfd *p_fds = calloc(max_connections, sizeof(struct pollfd));
    if (p_fds == NULL)
    {
        perror("Failed to allocate memory");
        return;
    }

    // Initialize all to -1. On some platforms fd can be larger than a byte so
    // its safest to loop over and set to -1.
    for (int fd_iter = 0; fd_iter < max_connections; fd_iter++)
    {
        p_fds[fd_iter].fd = -1;
    }
    p_fds[0].fd     = listening_socket;
    p_fds[0].events = POLLIN;

    const int buffer_size = BUFFER_SIZE;

    while (!shutdown_requested)
    {
        int poll_result = poll(p_fds, max_connections, -1);
        if (poll_result == -1)
        {
            if (errno == EINTR)
            {
                // Handling of interrupted system call.
                // Check if shutdown is requested or was interrupted some other
                // way not registered by the handler.
                printf("Poll interrupted by signal.\n");
                if (shutdown_requested)
                {
                    break; // Exit the loop for graceful shutdown.
                }
                continue; // Continue polling if not set to shutdown.
            }
            else
            {
                perror("Poll error");
                free(p_fds);
                return;
            }
        }

        if (p_fds[0].revents & POLLIN)
        {
            accept_new_connections(listening_socket, p_fds, max_connections);
        }

        process_existing_connections(
            p_fds, max_connections, buffer_size, p_url_queue);
    }

    printf("INFO: Starting Server Shutdown.\n");
    free(p_fds);
}

void
cleanup_server (int socket)
{
    close(socket);
}

// end of file.
