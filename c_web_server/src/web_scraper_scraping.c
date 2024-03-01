#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "web_scraper_scraping.h"
#include "web_scraper_utils.h"

#define CLIENT_PORT 80
#define BUFFER_SIZE 4096

static int resolve_hostname_to_ip(const char * p_hostname, struct sockaddr_in * p_serv_addr) {
    struct hostent * p_host = gethostbyname(p_hostname);
    if (p_host == NULL || p_host->h_addr_list[0] == NULL) {
        perror("DNS resolution failed");
        return EXIT_FAILURE;
    }

    memset(p_serv_addr, 0, sizeof(*p_serv_addr));
    p_serv_addr->sin_family = AF_INET;
    p_serv_addr->sin_port = htons(CLIENT_PORT);
    memcpy(&p_serv_addr->sin_addr, p_host->h_addr_list[0], p_host->h_length);

    return EXIT_SUCCESS;
}

static int create_and_connect_socket(const struct sockaddr_in * p_serv_addr) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Failed to create socket");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)p_serv_addr, sizeof(*p_serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    return sock;
}

static void send_http_get_request(int sock, const char * p_hostname, const char * p_path) {
    char request[1024];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", p_path, p_hostname);

    if (send(sock, request, strlen(request), 0) == -1) {
        perror("Failed to send message");
        close(sock);
    }
}

static int receive_http_response_and_write_to_file(int sock, const char * filename) {
    printf("Filename is %s.\n", filename);
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return EXIT_FAILURE;
    }
    FILE * p_file = fopen(filename, "w");
    if (!p_file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesRead] = '\0'; // Ensure null-termination
        fputs(buffer, p_file); // Write directly to file
    }
    if (bytesRead < 0) {
        perror("recv failed");
        return EXIT_FAILURE;
    }

    fclose(p_file);
    return EXIT_SUCCESS;
}

static int scrape_web_page (const char * p_url) {
    char filename[256];
    util_create_filename(p_url, filename, sizeof(filename));

    // Extract hostname and path
    char hostname[256] = {0};
    char path[512] = "/";
    sscanf(p_url, "http://%255[^/]%511[^\n]", hostname, path + 1); // +1 to skip initial '/'
    
    // Check if already scraped
    if (access(filename, F_OK) == 0) {
        printf("File already exists: %s,\n", filename);
        return EXIT_SUCCESS;
    }

    struct sockaddr_in serv_addr = {0};
    if (resolve_hostname_to_ip(hostname, &serv_addr) < 0) {
        return EXIT_FAILURE;
    }

    int sock = create_and_connect_socket(&serv_addr);
    if (sock < 0) {
        return EXIT_FAILURE;
    }

    send_http_get_request(sock, hostname, path);
    receive_http_response_and_write_to_file(sock, filename);

    close(sock);
    return EXIT_SUCCESS;
}

int handle_web_scrape (queue_t * p_url_queue)
{
    // Check if URL is on the queue to scrape
    char * p_url = queue_dequeue(p_url_queue);
    if (NULL == p_url)
    {
        fprintf(stderr, "URL to parse is NULL.\n");
        return EXIT_FAILURE;
    }
    // Scrape URL and write to disk
    if (scrape_web_page(p_url) == EXIT_FAILURE)
    {
        fprintf(stderr, "Failed to scrape %s.\n", p_url);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

// end of file.
