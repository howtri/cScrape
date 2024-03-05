// Queue data structure to store URLs to be scraped.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "web_scraper_queue.h"

typedef struct node
{
    struct node * p_next;
    char        * url;
} node_t;

typedef struct queue
{
    node_t * p_head;
    node_t * p_tail;
} queue_t;

// Create a node that contains a URL. Dynamic memory is used for both the node
// and the URL it contains. Node_destroy is responsible for freeing both.
static node_t *
node_create (char * p_url, int url_length)
{
    if ((NULL == p_url) || (url_length <= 0))
    {
        fprintf(stderr, "Invalid arguments.");
        return NULL;
    }

    // node_destroy is responsible for freeing this memory.
    node_t * new_node = calloc(1, sizeof(node_t));
    if (NULL == new_node)
    {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Handle dynamic memory for the URL to own its memory for the lifecycle
    // of the program. node_destroy is responsible for freeing it.
    char * url_mem = calloc(1, url_length + 1);
    if (NULL == url_mem)
    {
        perror("Failed to allocate memory");
        free(new_node);
        return NULL;
    }

    strncpy(url_mem, p_url, url_length);
    url_mem[url_length] = '\0';

    new_node->url = url_mem;
    return new_node;
}

// Frees a node and its contained url.
// The URL is freed here in the case that queue destroy is called.
static void
node_destroy (node_t ** pp_node)
{
    if ((NULL == pp_node) || (NULL == *pp_node))
    {
        fprintf(stderr, "Invalid pointer argument");
        return;
    }

    // Free them dynamic memory in the node for the URL.
    free((*pp_node)->url);
    (*pp_node)->url = NULL;

    // Free memory for the node and set to NULL to prevent
    // dangling pointers.
    free(*pp_node);
    *pp_node = NULL;
}

// Creates a queue datastructure, the dynamic memory allocated is
// expected to be freed in queue_destroy.
queue_t *
queue_create ()
{
    // queue_destroy is responsible for freeing the memory.
    queue_t *new_queue = calloc(1, sizeof(queue_t));
    if (NULL == new_queue)
    {
        perror("Failed to allocate memory");
        return NULL;
    }
    return new_queue;
}

// Makes calls to free every node contained in a queue and frees
// the queue data structure.
int
queue_destroy (queue_t ** pp_queue)
{
    if ((NULL == pp_queue) || (NULL == *pp_queue))
    {
        fprintf(stderr, "Invalid pointer argument");
        return EXIT_FAILURE;
    }

    // Free all nodes in the queue
    while ((*pp_queue)->p_head != NULL)
    {
        node_t *next_node = (*pp_queue)->p_head->p_next;
        node_destroy(&(*pp_queue)->p_head);
        (*pp_queue)->p_head = next_node;
    }

    free(*pp_queue);
    *pp_queue = NULL;
    return EXIT_SUCCESS;
}

// Creates a node for a URL and adds the node to the end (tail) of the queue.
int
queue_enqueue (queue_t * p_queue, char * p_url, int url_length)
{
    if (NULL == p_queue || NULL == p_url || url_length <= 0)
    {
        fprintf(stderr, "Invalid pointer argument or url_length\n");
        return EXIT_FAILURE;
    }

    // Create a new node with the provided URL.
    node_t * new_node = node_create(p_url, url_length);
    if (NULL == new_node)
    {
        fprintf(stderr, "Failed to create a new node\n");
        return EXIT_FAILURE;
    }

    if (NULL == p_queue->p_head)
    {
        p_queue->p_head = new_node;
        p_queue->p_tail = new_node;
    }
    else
    {
        p_queue->p_tail->p_next = new_node;
        p_queue->p_tail         = new_node;
    }

    return EXIT_SUCCESS;
}

// Removes a node and frees both the node and original URL. Allocates new memory
// for the URL that must be freed by the web scraper in handle_web_scrape.
char *
queue_dequeue (queue_t * p_queue)
{
    if (NULL == p_queue)
    {
        fprintf(stderr, "ERROR: Invalid pointer argument\n");
        return NULL;
    }

    // The queue being empty is an expected case. The worker threads
    // run until the queue is empty.
    if (NULL == p_queue->p_head)
    {
        return NULL;
    }

    // Remove the head node from the queue.
    node_t * removed_node = p_queue->p_head;
    // Duplicate URL, web_scraper_handler will free.
    char  * removed_node_url = removed_node->url;
    size_t len              = strlen(removed_node_url);
    char  * p_url            = calloc(1, len + 1); // +1 for the null terminator
    if (NULL == p_url)
    {
        perror("Failed to allocate memory");
        return NULL;
    }
    strcpy(p_url, removed_node_url);

    // Advance the head pointer.
    p_queue->p_head = p_queue->p_head->p_next;
    if (NULL == p_queue->p_head)
    { // If the queue is now empty, also reset the tail.
        p_queue->p_tail = NULL;
    }

    // Free the removed node.
    node_destroy(&removed_node);

    return p_url; // Return the duplicated URL.
}

// end of file.
