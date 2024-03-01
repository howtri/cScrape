// Include auxilary functions needed for the webscraper that dont fit
// into one specific component.

#include <stdio.h>
#include <stdlib.h>
#include "web_scraper_utils.h"

static size_t util_djb_hash(const char * p_cp) {
    if (p_cp == NULL) {
        return 0;
    }

    size_t hash = 5381;
    while (*p_cp) {
        hash = 33 * hash ^ (unsigned char) *p_cp++;
    }
    return hash;
}

int util_create_filename(const char * p_url, char * p_filename_buffer, size_t buffer_size) {    
    // Generate filename
    size_t hash = util_djb_hash(p_url);
    if (0 == hash)
    {
        return EXIT_FAILURE;
    }
    
    // Use buffer_size for snprintf
    snprintf(p_filename_buffer, buffer_size, "data/%zu.txt", hash);
    
    return EXIT_SUCCESS;
}

// end of file.
