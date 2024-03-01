// Include auxilary functions needed for the webscraper that dont fit
// into one specific component.

#include "web_scraper_utils.h"

size_t djb_hash(const char * p_cp) {
    size_t hash = 5381;
    while (*p_cp) {
        hash = 33 * hash ^ (unsigned char) *p_cp++;
    }
    return hash;
}

// end of file.
