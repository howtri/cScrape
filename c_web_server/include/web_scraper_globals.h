#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include "web_scraper_threads.h"

extern pthread_mutex_t global_queue_mutex;
extern thread_pool_t global_thread_pool;

/**
 * @brief Create global mutex and thread pool. Create threads in thread pool.
 * 
 * @return EXIT_FAILURE on failure, EXIT_SUCCESS on success.
 */
int init_globals(void);

/**
 * @brief Destroy mutex and thread pool (including all threads within).
 */
void destroy_globals(void);

#endif // GLOBALS_H

/*** end of file ***/
