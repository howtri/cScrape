#ifndef WEB_SCRAPER_SCRAPING_H
#define WEB_SCRAPER_SCRAPING_H

#include <netinet/in.h> // Required for struct sockaddr_in
#include "web_scraper_queue.h"


#define CLIENT_PORT 80
#define BUFFER_SIZE 4096

/**
 * Resolves a hostname to an IP address and prepares the sockaddr_in structure.
 * 
 * @param hostname The hostname to resolve.
 * @param serv_addr A pointer to the sockaddr_in structure to populate with the server's address.
 * @return int Returns EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int resolve_hostname_to_ip(const char* hostname, struct sockaddr_in* serv_addr);

/**
 * Creates a socket and connects it to the specified server.
 * 
 * @param serv_addr A pointer to the sockaddr_in structure containing the server's address.
 * @return int A socket descriptor on success, -1 on failure.
 */
int create_and_connect_socket(const struct sockaddr_in* serv_addr);

/**
 * Sends an HTTP GET request to the server using the specified socket.
 * 
 * @param sock The socket descriptor to use for sending the request.
 * @param hostname The hostname of the server, used for the Host header in the HTTP request.
 * @param path The path to request, starting with '/'.
 */
void send_http_get_request(int sock, const char* hostname, const char* path);

/**
 * Receives the HTTP response from the server and writes to a file.
 * 
 * @param sock The socket descriptor to use for receiving the response.
 * @param filename The file to write to including path.
 * @return int Returns EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int receive_http_response_and_write_to_file(int sock, const char* filename);

/**
 * High-level function to scrape a web page given its URL.
 * 
 * @param url The URL of the web page to scrape.
 * @return int Returns EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int scrape_web_page(const char* url);

/**
 * Handler function to be executed by threads to detect
 * urls to be scraped from the queue and initiate scrape_web_page.
 * 
 * @param url The queue containing URLs.
 * @return int Returns EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int handle_web_scrape (queue_t * p_url_queue);

#endif /* WEB_SCRAPER_SCRAPING_H */

/*** end of file ***/
