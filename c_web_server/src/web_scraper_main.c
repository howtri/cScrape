// Main file for the web server web scraper.

#include <stdio.h>
#include <stdlib.h>
#include "web_scraper_server.h"
#include "web_scraper_queue.h"
#include "web_scraper_globals.h"

#define MAX_CONNECTIONS 10

int
main ()
{
    // Creates an IPv4 socket and beings listening for connections.
    int listening_socket = start_server(MAX_CONNECTIONS);
    if (listening_socket < 0)
    {
        fprintf(stderr, "Failed to create a listening socket.\n");
        return EXIT_FAILURE;
    }

    // Create URL queue for the webserver to place URLs for processing on client
    // request.
    queue_t *p_url_queue = queue_create();
    if (NULL == p_url_queue)
    {
        fprintf(stderr, "Failed to create queue.\n");
        cleanup_server(listening_socket);
        return EXIT_FAILURE;
    }

    // Creates our thread pool. Currently the thread pool is only used for
    // webscraping tasks.
    if (init_globals() == EXIT_FAILURE)
    {
        cleanup_server(listening_socket);
        return EXIT_FAILURE;
    }

    // Continually monitors for new and established connections for any
    // processing that can take place. Accepts up to MAX_CONNECTIONS at a time
    // before load-shedding by dropping any new connections that are made until
    // existing connections complete.
    handling_loop(listening_socket, MAX_CONNECTIONS, p_url_queue);

    // Joins all of the threads and destroys the threadpool + mutex. This needs
    // to be done before the queue is destroyed for any threads that are still
    // processing.
    destroy_globals();

    // Free the url queue and all nodes within it, including all url strings.
    if (queue_destroy(&p_url_queue) == EXIT_FAILURE)
    {
        fprintf(stderr, "Failed to destroy the queue.\n");
        return EXIT_FAILURE;
    }
    p_url_queue = NULL;

    // Free the socket the webserver is listening on.
    cleanup_server(listening_socket);
    return EXIT_SUCCESS;
}

// end of file.
