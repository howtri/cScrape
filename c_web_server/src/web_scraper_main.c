// Main file for the web server web scraper.

#include <stdio.h>
#include <stdlib.h>
#include "web_scraper_server.h"

#define MAX_CONNECTIONS 10

int
main ()
{
    int listening_socket = start_server(MAX_CONNECTIONS);
    if (listening_socket < 0)
    {
        fprintf(stderr, "Failed to create a listening socket.\n");
        return EXIT_FAILURE;
    }
    handling_loop(listening_socket, MAX_CONNECTIONS);
    cleanup_server(listening_socket);
    return EXIT_SUCCESS;
}

// end of file.
