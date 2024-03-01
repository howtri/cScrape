#ifndef WEB_SCRAPER_HANDLERS_H
#define WEB_SCRAPER_HANDLERS_H

#include "web_scraper_queue.h"

/**
 * @brief Handles a new scrape request by validating the URL and enqueueing it.
 * 
 * Validates the provided URL to ensure it starts with a valid scheme (http:// or https://).
 * If the URL is valid, it is enqueued in the provided URL queue for processing.
 * 
 * @param p_url A pointer to the URL string to be validated and enqueued.
 * @param p_url_queue A pointer to the queue in which the URL should be enqueued.
 * @return int EXIT_SUCCESS if the URL is successfully enqueued; otherwise, EXIT_FAILURE.
 */
int handle_scrape_new_request(char *p_url, queue_t *p_url_queue);

/**
 * @brief Handles a request to return the content of a previously scraped page.
 * 
 */
void handle_return_scrape_request();

#endif /*** WEB_SCRAPER_HANDLERS_H ***/

/*** end of file ***/
