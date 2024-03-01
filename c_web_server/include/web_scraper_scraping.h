#ifndef WEB_SCRAPER_SCRAPING_H
#define WEB_SCRAPER_SCRAPING_H

#include <netinet/in.h> // Required for struct sockaddr_in
#include "web_scraper_queue.h"

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
