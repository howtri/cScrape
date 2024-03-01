// Main file for the web server web scraper.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web_scraper_queue.h"
#include "web_scraper_handlers.h"

int handle_scrape_new_request (char * p_url, queue_t * p_url_queue)
{
    // Validate the url.
    if ((NULL == p_url) || (NULL == p_url_queue))
    {
        fprintf(stderr, "Invalid pointer arguments.\n");
        return EXIT_FAILURE;
    }

    // TODO: Regex
    if (!((strncmp(p_url, "http://", 7) == 0) || (strncmp(p_url, "https://", 8) == 0)))
    {
        printf("URL does not start with a valid scheme.\n");
        return EXIT_FAILURE;
    }

    // Enqueue valid url
    return queue_enqueue(p_url_queue, p_url, strlen(p_url));
}

void handle_return_scrape_request ()
{
    // Check that a file is present for the scrape request.

    // Read from the file

    // Return the buffer
    return;
}

// end of file.
