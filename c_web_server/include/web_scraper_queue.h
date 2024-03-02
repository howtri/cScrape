#ifndef WEB_SCRAPER_QUEUE_H
#define WEB_SCRAPER_QUEUE_H

typedef struct queue queue_t;

/**
 * @brief Creates a new queue.
 * 
 * @return queue_t* Pointer to the newly created queue, or NULL on failure.
 */
queue_t *queue_create(void);

/**
 * @brief Frees a queue and all its nodes.
 * 
 * @param pp_queue Double pointer to the queue to be freed.
 * @return int EXIT_SUCCESS on success, or EXIT_FAILURE on failure.
 */
int queue_destroy(queue_t ** pp_queue);

/**
 * @brief Enqueues a URL into the queue.
 * 
 * @param p_queue Pointer to the queue.
 * @param p_url Pointer to the URL string to be enqueued.
 * @param url_length Length of the URL string.
 * @return int EXIT_SUCCESS on success, or EXIT_FAILURE on failure.
 */
int queue_enqueue(queue_t * p_queue, char * p_url, int url_length);

/**
 * @brief Dequeues a URL from the queue.
 * 
 * @param p_queue Pointer to the queue.
 * @return char* Pointer to the dequeued URL string, or NULL if the queue is empty.
 * The caller is responsible for freeing the returned string.
 */
char *queue_dequeue(queue_t * p_queue);

#endif /** WEB_SCRAPER_QUEUE_H **/
