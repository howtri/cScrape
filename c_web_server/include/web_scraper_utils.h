#ifndef WEB_SCRAPER_UTILS_H
#define WEB_SCRAPER_UTILS_H

#include <stdio.h>

/**
 * @brief D. J. Bernstein hash function.
 * Source: https://codereview.stackexchange.com/questions/85556/simple-string-hashing-algorithm-implementation
 * 
 * @param p_cp The string to hash.
 * @return hash.
 */
size_t djb_hash(const char * p_cp);

#endif /*** WEB_SCRAPER_UTILS_H ***/

/*** end of file ***/
