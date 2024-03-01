#ifndef WEB_SCRAPER_SERVER_H
#define WEB_SCRAPER_SERVER_H

#include "web_scraper_queue.h"

/**
 * Starts the server, creating a socket and listening for connections.
 * 
 * @return int The socket file descriptor of the listening socket, or -1 on error.
 */
int start_server(int max_connections);

/**
 * Cleans up server resources associated with the socket.
 * 
 * @param socket The socket file descriptor to clean up.
 * @return int Returns 0 on success.
 */
int cleanup_server(int socket);

/**
 * Main loop for handling connections.
 * 
 * @param listening_socket The listening socket's file descriptor.
 * @param max_connections The maximum number of simultaneous connections.
 */
void handling_loop(int listening_socket, int max_connections, queue_t * p_url_queue);

#endif /* WEB_SCRAPER_SERVER_H */

/*** end of file ***/
