#ifndef WEB_SCRAPER_SCRAPING_H
#define WEB_SCRAPER_SCRAPING_H

#include <netinet/in.h> // Required for struct sockaddr_in
#include "web_scraper_queue.h"

/**
 * Connects to a HTTP host as a client, sends a simple HTTP request,
 * writes the reply to disk on the local machine.
 * 
 * @param p_url The URL to scrape.
 * @return int Returns EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int scrape_web_page (const char * p_url);

#endif /* WEB_SCRAPER_SCRAPING_H */

/*** end of file ***/
