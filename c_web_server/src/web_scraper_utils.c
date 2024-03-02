// Include auxilary functions needed for the webscraper that dont fit
// into one specific component.

#include <stdio.h>
#include <stdlib.h>
#include "web_scraper_utils.h"

// Simple djb hash implementation for fast non cryptographical hashing
// https://codereview.stackexchange.com/questions/85556/simple-string-hashing-algorithm-implementation
static size_t
util_djb_hash (const char *p_cp)
{
    if (p_cp == NULL)
    {
        return 0;
    }

    size_t hash = 5381;
    while (*p_cp)
    {
        hash = 33 * hash ^ (unsigned char)*p_cp++;
    }
    return hash;
}

int
util_create_filename (const char *p_url,
                      char       *p_filename_buffer,
                      size_t      buffer_size)
{
    // Generate filename
    size_t hash = util_djb_hash(p_url);
    if (0 == hash)
    {
        return EXIT_FAILURE;
    }

    // Formats specifically for a Linux based file system.
    snprintf(p_filename_buffer, buffer_size, "data/%zu.txt", hash);

    return EXIT_SUCCESS;
}

// end of file.
