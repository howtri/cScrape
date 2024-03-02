// Main file for the web server web scraper.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include "web_scraper_queue.h"
#include "web_scraper_handlers.h"
#include "web_scraper_utils.h"

#define MESSAGE_SIZE 1024

// Handle opening a file and error checking.
static FILE *
open_file (const char *p_filename)
{
    if (access(p_filename, F_OK) != 0)
    {
        return NULL;
    }

    FILE *p_file = fopen(p_filename, "r");
    if (!p_file)
    {
        perror("Failed to open file");
        return NULL;
    }
    return p_file;
}

// Reads and sends all of a files contents in max transmitions of MESSAGE_SIZE bytes in
// a loop.
static int
send_file_contents (int socket_fd, FILE *p_file)
{
    char    buffer[MESSAGE_SIZE]; // Adjust buffer size as needed
    ssize_t bytes_read = 0;
    ssize_t bytes_sent = 0;

    // Continue reading until EOF.
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), p_file)) > 0)
    {
        char *p_ptr = buffer;
        while (bytes_read > 0)
        {
            bytes_sent = send(socket_fd, p_ptr, bytes_read, 0);
            if (bytes_sent < 0)
            {
                // Blocking condition, try again.
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    continue;
                }
                perror("Failed to send");
                return EXIT_FAILURE;
            }
            bytes_read -= bytes_sent;
            p_ptr += bytes_sent;
        }
    }

    if (ferror(p_file))
    {
        perror("Failed to read from file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int
handle_scrape_new_request (int socket_fd, char *p_url, queue_t *p_url_queue)
{
    // Validate the url.
    if ((NULL == p_url) || (NULL == p_url_queue))
    {
        fprintf(stderr, "Invalid pointer arguments.\n");
        return EXIT_FAILURE;
    }

    // TODO: Regex
    if (!((strncmp(p_url, "http://", 7) == 0)
          || (strncmp(p_url, "https://", 8) == 0)))
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
    if (send(socket_fd, response, strlen(response), 0) == -1)
    {
        perror("send failed");
    }
    return EXIT_SUCCESS;
}

int
handle_return_scrape_request (int socket_fd, char *p_url)
{
    char filename[256];
    util_create_filename(p_url, filename, sizeof(filename));

    FILE *file = open_file(filename);
    if (!file)
    {
        printf(
            "URL has not been scraped before, please request it be scraped "
            "%s,\n",
            p_url);
        // Request still handled successfully in this case.
        char response[]
            = "SUCCESS: URL has not been scraped before, please request it be "
              "scraped.\n";
        if (send(socket_fd, response, strlen(response), 0) == -1)
        {
            perror("send failed");
            return EXIT_FAILURE;
        }
    }
    int result = send_file_contents(socket_fd, file);
    fclose(file);

    if (result == EXIT_FAILURE)
    {
        fprintf(stderr, "Failed to send scraped contents.\n");
        const char *msg = "FAILURE: Failed to send scraped contents.\n";
        send(socket_fd, msg, strlen(msg), 0);
    }

    return EXIT_SUCCESS;
}

int
handle_invalid_request (int socket_fd)
{
    char response[] = "SUCCESS: Invalid option, expected 1 or 2.\n";
    if (send(socket_fd, response, strlen(response), 0) == -1)
    {
        perror("send failed");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// end of file.
