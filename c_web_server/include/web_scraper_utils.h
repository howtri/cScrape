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
size_t util_djb_hash(const char * p_cp);

/**
 * @brief Create a filename including path based on a URLs hash.
 * 
 * @param p_url The url to be hash.
 * @param p_filename_buffer The buffer to write the filename to.
 * @param buffer_size Size of the buffer.
 * @return EXIT_FAILURE on failure, EXIT_SUCCESS on success.
 */
int util_create_filename(const char *p_url, char *p_filename_buffer, size_t buffer_size);

#endif /*** WEB_SCRAPER_UTILS_H ***/

/*** end of file ***/
