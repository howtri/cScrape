#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_TASKS 100

typedef void *(*task_function)(void *);

typedef struct task {
    task_function func;
    void *arg;
} task_t;

typedef struct thread_pool {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_t *threads;
    task_t tasks[MAX_TASKS];
    int task_count;
    int pool_size;
    bool stop;
} thread_pool_t;

/**
 * @brief Initializes a thread pool with a specified number of worker threads.
 * 
 * This function sets up the thread pool structure, creates worker threads, and prepares it for accepting tasks.
 * 
 * @param pool Pointer to the thread pool structure to be initialized.
 * @param num_threads The number of worker threads to create in the thread pool.
 * @return 0 on success, -1 on failure to initialize the thread pool or create threads.
 */
int thread_pool_init(thread_pool_t *p_pool, int num_threads);

/**
 * @brief Adds a new task to the thread pool for execution.
 * 
 * Queues a task to be executed by the thread pool. The task is defined by a function pointer
 * and an argument to be passed to that function. The task will be executed by an available worker thread.
 * 
 * @param pool Pointer to an initialized thread pool where the task will be added.
 * @param func The task function to be executed by a thread in the pool.
 * @param arg The argument to be passed to the task function.
 * @return 0 on successfully adding the task to the pool, -1 if the task could not be added.
 */
int thread_pool_add_task(thread_pool_t *p_pool, task_function func, void *p_arg);

/**
 * @brief Cleans up the thread pool and terminates all its worker threads.
 * 
 * Stops all worker threads, waits for them to finish their current tasks, and then cleans up
 * all resources associated with the thread pool. This function should be called to avoid resource leaks
 * when the thread pool is no longer needed.
 * 
 * @param pool Pointer to the thread pool to be destroyed.
 */
void thread_pool_destroy(thread_pool_t *p_pool);

/**
 * @brief Task function that continuously processes URLs from a queue until a NULL URL is encountered.
 * 
 * Intended to be run by a worker thread in a thread pool. This function dequeues URLs from a specified queue,
 * processes them by scraping content from the web, and repeats this process until it encounters a NULL URL,
 * indicating no more URLs are available for processing. The function uses a global mutex to ensure thread-safe
 * access to the queue.
 * 
 * @param arg A pointer to the queue_t structure from which URLs will be dequeued and processed.
 * @return NULL always, as the return value is not used in this context.
 */
void *scrape_url_task(void *p_arg);

#endif // THREAD_POOL_H
