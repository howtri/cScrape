// Demonstrate skill in using secure coding techniques

#include "template_main.h"
#include <stdio.h>

#define MAX_SIZE 10

int
main ()
{
    void * p_safe_buffer = safe_alloc(MAX_SIZE);
    free(p_safe_buffer);
    p_safe_buffer = NULL;

    return EXIT_SUCCESS;
}

// Safe alloc ensures a buffer of size of contents + 1 is created
// to ensure the string is null-terminated.
void *
safe_alloc (size_t num_bytes)
{
    void * p_safe_buffer = calloc(1, num_bytes + 1);
    if (NULL == p_safe_buffer)
    {
        perror("Failed to allocate memory");
        return NULL;
    }
    return p_safe_buffer;
}

// end of file.
