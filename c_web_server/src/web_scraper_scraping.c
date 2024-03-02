// Contains logic to scrape a website by sending a simple HTTP request as the client.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "web_scraper_scraping.h"
#include "web_scraper_utils.h"

#define CLIENT_PORT "80"
#define BUFFER_SIZE 4096
#define FILENAME_SIZE 256
#define PATH_SIZE 512

// Attempt to resolve a hostname to an IPv4 address using getaddrinfo.
static int
resolve_hostname_to_ip(const char *p_hostname, struct sockaddr_in *p_serv_addr)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // We must maintain track of response head to free the structure.
    struct addrinfo *p_res_head = NULL;
    struct addrinfo *p_res = NULL; 

    // getaddrinfo is thread safe.
    int status = getaddrinfo(p_hostname, CLIENT_PORT, &hints, &p_res_head);
    if (0 != status)
    {
        // gai_strerror() function translates these error codes to a
        // human readable string, suitable for error reporting.
        fprintf(stderr, "DNS resolution failed: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }

    for (p_res = p_res_head; p_res != NULL; p_res = p_res->ai_next)
    {
        if (p_res->ai_family == AF_INET) // Check for IPv4 address
        {
            // Copy the IPv4 response into our address structure.
            *p_serv_addr = *(struct sockaddr_in *)p_res->ai_addr;
            freeaddrinfo(p_res_head); // Free the response linked list
            return EXIT_SUCCESS;
        }
    }

    // No IPv4 addresses were found
    fprintf(stderr, "Failed to find IPv4 address for %s\n", p_hostname);
    freeaddrinfo(p_res_head);
    return EXIT_FAILURE;
}

// Connect to the site we are scraping.
static int
create_and_connect_socket (const struct sockaddr_in *p_serv_addr)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Failed to create socket");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)p_serv_addr, sizeof(*p_serv_addr)) < 0)
    {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    return sock;
}

// Send the HTTP request to get the page over our TCP connection.
static void
send_http_get_request (int sock, const char * p_hostname, const char * p_path)
{
    char request[1024];
    snprintf(request,
             sizeof(request),
             "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
             p_path,
             p_hostname);

    if (send(sock, request, strlen(request), 0) == -1)
    {
        perror("Failed to send message");
        close(sock);
    }
}

// Receive the response over the TCP connection and write to disk.
static int
receive_http_response_and_write_to_file (int sock, const char * filename)
{
    printf("Filename is %s.\n", filename);

    FILE *p_file = fopen(filename, "w");
    if (!p_file)
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char    buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[bytesRead] = '\0';
        // Write directly to the file to limit memory usage.
        fputs(buffer, p_file);
    }
    if (bytesRead < 0)
    {
        perror("Failed to recv from the server");
        fclose(p_file);
        return EXIT_FAILURE;
    }

    fclose(p_file);
    return EXIT_SUCCESS;
}

int
scrape_web_page (const char * p_url)
{
    if (NULL == p_url)
    {
        fprintf(stderr, "Null pointer argument.\n");
        return EXIT_FAILURE;
    }

    char filename[FILENAME_SIZE];
    util_create_filename(p_url, filename, sizeof(filename));

    // Extract hostname and path
    char hostname[FILENAME_SIZE] = { 0 };
    char path[PATH_SIZE]     = "/";
    // NOTE: Magic number use below. If FILENAME_SIZE or PATH_SIZE are changed
    // then the below parsing string must also reflect.
    // MUST USE: FILENAME_SIZE -1 and PATH_SIZE -1.
    sscanf(p_url,
           "http://%255[^/]%511[^\n]",
           hostname,
           path + 1); // +1 to skip initial '/'

    // Check if already scraped
    if (access(filename, F_OK) == 0)
    {
        printf("File already exists: %s,\n", filename);
        return EXIT_SUCCESS;
    }

    struct sockaddr_in serv_addr = { 0 };
    if (resolve_hostname_to_ip(hostname, &serv_addr) < 0)
    {
        return EXIT_FAILURE;
    }

    int sock = create_and_connect_socket(&serv_addr);
    if (sock < 0)
    {
        return EXIT_FAILURE;
    }

    send_http_get_request(sock, hostname, path);
    receive_http_response_and_write_to_file(sock, filename);

    close(sock);
    return EXIT_SUCCESS;
}

// end of file.
