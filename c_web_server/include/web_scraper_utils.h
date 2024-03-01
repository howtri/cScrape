#ifndef WEB_SCRAPER_UTILS_H
#define WEB_SCRAPER_UTILS_H

/**
 * @brief Create a filename including path based on a URLs hash.
 * 
 * @param p_url The url to be hash.
 * @param p_filename_buffer The buffer to write the filename to.
 * @param buffer_size Size of the buffer.
 * @return EXIT_FAILURE on failure, EXIT_SUCCESS on success.
 */
int util_create_filename(const char * p_url, char * p_filename_buffer, size_t buffer_size);

#endif /*** WEB_SCRAPER_UTILS_H ***/

/*** end of file ***/
