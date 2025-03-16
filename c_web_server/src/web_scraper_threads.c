// Contains logic for thread pool creation and management. Contains logic for
// threading tasks to be processed.

#include <stdlib.h>
#include <stdio.h>
#include "web_scraper_threads.h"
#include "web_scraper_queue.h"
#include "web_scraper_globals.h"
#include "web_scraper_scraping.h"

// Threads run the worker thread and wait for any tasks to become
// available from thread_pool_add_task. Only the primary thread adds
// and removes tasks for the thread pool.
static void *
worker_thread (void * p_arg)
{
    thread_pool_t *p_pool = p_arg;

    while (true)
    {
        // Lock around the task queue so other threads don't grab the same
        // task.
        pthread_mutex_lock(&p_pool->lock);

        // Block while there are no tasks and not signaled to stop.
        while ((p_pool->task_count == 0) && (!p_pool->stop))
        {
            pthread_cond_wait(&p_pool->cond, &p_pool->lock);
        }

        // Exit thread condition.
        if (p_pool->stop)
        {
            pthread_mutex_unlock(&p_pool->lock);
            break;
        }

        // Pop the head task and shift all to the left.
        // We could use a queue in this case also but due to a low
        // number of threads its likely not neccesary.
        task_t task = p_pool->tasks[0];
        for (int iter = 0; iter < p_pool->task_count - 1; iter++)
        {
            p_pool->tasks[iter] = p_pool->tasks[iter + 1];
        }
        p_pool->task_count--;
        pthread_mutex_unlock(&p_pool->lock);

        task.func(task.arg);
    }

    return NULL;
}

int
thread_pool_init (thread_pool_t * p_pool, int num_threads)
{
    if (pthread_mutex_init(&p_pool->lock, NULL) != 0)
    {
        return EXIT_FAILURE;
    }
    if (pthread_cond_init(&p_pool->cond, NULL) != 0)
    {
        pthread_mutex_destroy(&p_pool->lock);
        return EXIT_FAILURE;
    }

    p_pool->threads = calloc(num_threads, sizeof(pthread_t));
    if (NULL == p_pool->threads)
    {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }
    p_pool->pool_size  = 0;
    p_pool->task_count = 0;
    p_pool->stop       = false;

    for (int iter = 0; iter < num_threads; iter++)
    {
        if (pthread_create(&p_pool->threads[iter], NULL, worker_thread, p_pool)
            != 0)
        {
            // Cleanup and exit if thread creation fails for any thread.
            thread_pool_destroy(p_pool);
            return EXIT_FAILURE;
        }
        // Only track how many threads were succesfully allocated in the case
        // thread_pool_destroy is called early on pthread_create failure.
        p_pool->pool_size++;
    }

    return EXIT_SUCCESS;
}

int
thread_pool_add_task (thread_pool_t * p_pool, task_function func, void * p_arg)
{
    pthread_mutex_lock(&p_pool->lock);

    if (p_pool->task_count >= MAX_TASKS)
    {
        pthread_mutex_unlock(&p_pool->lock);
        // Task queue is full, no additional tasks should be added.
        // In some cases this may mean denying client requests.
        return EXIT_FAILURE;
    }

    p_pool->tasks[p_pool->task_count++]
        = (task_t) { .func = func, .arg = p_arg };
    pthread_cond_signal(&p_pool->cond);
    pthread_mutex_unlock(&p_pool->lock);

    return EXIT_SUCCESS;
}

void
thread_pool_destroy (thread_pool_t * p_pool)
{
    // Signal for all threads to stop and exit the worker thread function.
    pthread_mutex_lock(&p_pool->lock);
    p_pool->stop = true;
    pthread_cond_broadcast(&p_pool->cond);
    pthread_mutex_unlock(&p_pool->lock);

    // Close all non-primary threads. No information is retrieved from the
    // threads.
    for (int iter = 0; iter < p_pool->pool_size; iter++)
    {
        pthread_join(p_pool->threads[iter], NULL);
    }

    free(p_pool->threads);
    pthread_mutex_destroy(&p_pool->lock);
    pthread_cond_destroy(&p_pool->cond);
}

// Scrapes URLs from the URL queue. Continues scraping incrementally until there
// are no URLs currently on the queue and then will end. This task is added to
// the queue everytime a URL is enqueued but purposefully will continue
// processing URLs in the case that the task queue gets filled but there are
// still extra URLs to process. Responsible for freeing the dynamic memory for
// the URL that is allocated in queue dequeue.
void *
scrape_url_task_thread (void * p_arg)
{
    queue_t *p_url_queue = p_arg;
    while (true)
    {
        // Critical Path: Ensure the queue is not enqueued or dequeued by
        // multiple threads.
        pthread_mutex_lock(&global_queue_mutex);
        // Attempt to dequeue a URL. If the queue is empty, p_url will be NULL.
        char *p_url = queue_dequeue(p_url_queue);
        pthread_mutex_unlock(&global_queue_mutex);

        // If no URL is available, exit this task function.
        if (p_url == NULL)
        {
            break; // Exit the loop and end the task
        }

        // Scrape URL and write to disk
        if (scrape_web_page(p_url) == EXIT_FAILURE)
        {
            fprintf(stderr, "Failed to scrape %s.\n", p_url);
        }

        free(p_url); // Always free the URL after processing
        queue_enqueue(p_url_queue, p_url, 10);
    }

    return NULL;
}

// end of file.
