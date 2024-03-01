// Main file for the web server web scraper.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "web_scraper_queue.h"
#include "web_scraper_handlers.h"
#include "web_scraper_utils.h"

int handle_scrape_new_request (int socket_fd, char * p_url, queue_t * p_url_queue)
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
    if (queue_enqueue(p_url_queue, p_url, strlen(p_url)) == EXIT_FAILURE)
    {
        fprintf(stderr, "Failed to add URL %s to the queue.\n", p_url);
        return EXIT_FAILURE;
    }

    // Reply to the client
    char response[] = "SUCCESS: URL queued to be scraped.\n";
    if (send(socket_fd, response, strlen(response), 0) == -1) {
            perror("send failed");
    }
    return EXIT_SUCCESS;
}

int handle_return_scrape_request (int socket_fd, char * p_url)
{
    // Generate filename of requested url.
    char filename[256];
    util_create_filename(p_url, filename, sizeof(filename));

    // Read from the file
    if (access(filename, F_OK) != 0) {
        printf("URL has not been scraped before, please request it be scraped %s,\n", p_url);
        return EXIT_SUCCESS;
    }
    // Return the buffer

    return EXIT_SUCCESS;
}

int handle_invalid_request(int socket_fd)
{
    return EXIT_SUCCESS;
}

// end of file.
