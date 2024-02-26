#ifndef TEMPLATE_MAIN_H
#define TEMPLATE_MAIN_H

// stdio is included in the .c file since its used only in main.
#include <stdlib.h>

#define MAX_SIZE 10

/**
 * @brief Allocates a buffer safely, ensuring it is null-terminated.
 * 
 * @param num_bytes The number of bytes to allocate, plus one for the null terminator.
 * @return void* A pointer to the allocated buffer.
 */
void * safe_alloc (size_t num_bytes);

#endif /*** TEMPLATE_MAIN_H ***/

/*** end of file ***/
