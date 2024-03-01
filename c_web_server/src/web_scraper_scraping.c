#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "web_scraper_scraping.h"

#define PORT 80
#define BUFFER_SIZE 4096

int resolve_hostname_to_ip(const char* hostname, struct sockaddr_in* serv_addr) {
    struct hostent* host = gethostbyname(hostname);
    if (host == NULL || host->h_addr_list[0] == NULL) {
        perror("DNS resolution failed");
        return -1;
    }

    memset(serv_addr, 0, sizeof(*serv_addr));
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(PORT);
    memcpy(&serv_addr->sin_addr, host->h_addr_list[0], host->h_length);

    return 0;
}

int create_and_connect_socket(const struct sockaddr_in* serv_addr) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Failed to create socket");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)serv_addr, sizeof(*serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    return sock;
}

void send_http_get_request(int sock, const char* hostname, const char* path) {
    char request[1024];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, hostname);

    if (send(sock, request, strlen(request), 0) == -1) {
        perror("Failed to send message");
        close(sock);
    }
}

void receive_http_response(int sock) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    printf("Scraped content:\n\n");
    while ((bytesRead = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesRead] = '\0'; // Ensure null-termination
        printf("%s", buffer);
    }
    if (bytesRead < 0) {
        perror("recv failed");
    }
    printf("\nEnd of content.\n");
}

int scrape_web_page(const char* url) {
    char hostname[256] = {0};
    char path[512] = "/";
    sscanf(url, "http://%255[^/]%511[^\n]", hostname, path);

    struct sockaddr_in serv_addr;
    if (resolve_hostname_to_ip(hostname, &serv_addr) < 0) {
        return -1;
    }

    int sock = create_and_connect_socket(&serv_addr);
    if (sock < 0) {
        return -1;
    }

    send_http_get_request(sock, hostname, path);
    receive_http_response(sock);

    close(sock);
    return 0;
}

// end of file.
