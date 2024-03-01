// Main file for the web server web scraper.

#include <stdio.h>
#include <stdlib.h>
#include "web_scraper_server.h"
#include "web_scraper_queue.h"
#include "web_scraper_scraping.h"

#define MAX_CONNECTIONS 10

int
main ()
{
    // Web Server Threads.
    int listening_socket = start_server(MAX_CONNECTIONS);
    if (listening_socket < 0)
    {
        fprintf(stderr, "Failed to create a listening socket.\n");
        return EXIT_FAILURE;
    }

    // Create URL queue.
    queue_t *p_url_queue = queue_create();
    if (NULL == p_url_queue)
    {
        fprintf(stderr, "Failed to create queue");
        cleanup_server(listening_socket);
        return EXIT_FAILURE;
    }

    handling_loop(listening_socket, MAX_CONNECTIONS, p_url_queue);

    // Web Scraping Threads.
    int s_code = handle_web_scrape(p_url_queue);
    printf("Returned %d.\n", s_code);

    // Clean-up
    queue_destroy(&p_url_queue);
    cleanup_server(listening_socket);
    return EXIT_SUCCESS;
}

// end of file.
