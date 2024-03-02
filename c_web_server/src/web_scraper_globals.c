// Define global variables needed across multiple files.

#include "web_scraper_globals.h"

pthread_mutex_t global_queue_mutex;
thread_pool_t global_thread_pool;

#define THREAD_COUNT 5

// Create our queue mutex and thread pool to be used by our handlers
// and threading functions. Create thread pool for worker functions.
int init_globals() {
    pthread_mutex_init(&global_queue_mutex, NULL);
    if (thread_pool_init(&global_thread_pool, THREAD_COUNT) == EXIT_FAILURE)
    {
        pthread_mutex_destroy(&global_queue_mutex);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// Clean up mutex and all threads.
void destroy_globals() {
    pthread_mutex_destroy(&global_queue_mutex);
    thread_pool_destroy(&global_thread_pool);
}

// end of file.
