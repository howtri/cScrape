#ifndef WEB_SCRAPER_SERVER_H
#define WEB_SCRAPER_SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

/**
 * Initializes server address settings.
 * 
 * @return struct sockaddr_in Initialized server address structure.
 */
struct sockaddr_in initialize_server_address(void);

/**
 * Starts the server, creating a socket and listening for connections.
 * 
 * @return int The socket file descriptor of the listening socket, or -1 on error.
 */
int start_server(void);

/**
 * Cleans up server resources associated with the socket.
 * 
 * @param socket The socket file descriptor to clean up.
 * @return int Returns 0 on success.
 */
int cleanup_server(int socket);

/**
 * Accepts a new connection on the given socket.
 * 
 * @param socket_fd The file descriptor of the socket on which to accept the connection.
 * @return int The file descriptor of the accepted connection, or -1 on failure.
 */
int accept_new_connection(int socket_fd);

/**
 * Accepts new connections and manages them using poll file descriptors.
 * 
 * @param listening_socket The listening socket's file descriptor.
 * @param p_fds An array of struct pollfd for managing multiple connections.
 * @param max_connections The maximum number of connections to manage.
 */
void accept_new_connections(int listening_socket, struct pollfd p_fds[], int max_connections);

/**
 * Receives a message from a connection.
 * 
 * @param connection_socket The socket file descriptor from which to receive the message.
 * @param p_buffer The buffer to store the received message.
 * @param buffer_size The size of the buffer.
 * @return ssize_t The number of bytes received, or -1 on error.
 */
ssize_t receive_message(int connection_socket, char *p_buffer, size_t buffer_size);

/**
 * Processes existing connections, reading messages where available.
 * 
 * @param p_fds An array of struct pollfd representing the current connections.
 * @param max_connections The maximum number of connections to process.
 * @param buffer_size The size of the buffer used for receiving messages.
 */
void process_existing_connections(struct pollfd p_fds[], int max_connections, int buffer_size);

/**
 * Main loop for handling connections.
 * 
 * @param listening_socket The listening socket's file descriptor.
 * @param max_connections The maximum number of simultaneous connections.
 */
void handling_loop(int listening_socket, int max_connections);

#endif /* WEB_SCRAPER_SERVER_H */

/*** end of file ***/
