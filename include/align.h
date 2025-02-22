/**
 * @file align.h
 * @brief Provides functions for memory alignment.
 *
 * This file contains functions to align memory to a specified alignment requirement.
 * The alignment must be a power of 2 and at least sizeof(void*).
 */

#ifndef IMSDL_ALIGN_H
#define IMSDL_ALIGN_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#ifdef _WIN32
    #include <malloc.h>
#else
    #include <errno.h>
#endif

/**
 * @brief Aligns a pointer to the specified alignment.
 *
 * @param ptr The pointer to the memory to align.
 * @param alignment The alignment requirement, must be a power of 2 and at least sizeof(void*).
 * @return A pointer to the aligned memory, or NULL if alignment fails.
 */
void* aligned_pointer(void* ptr, size_t alignment);

/**
 * @brief Allocates aligned memory.
 *
 * On POSIX systems, it uses posix_memalign.
 * On Windows, it uses _aligned_malloc.
 *
 * @param alignment The alignment requirement, must be a power of 2 and at least sizeof(void*).
 * @param size The size of the memory block to allocate.
 * @return A pointer to the aligned memory, or NULL if allocation fails.
 */
void* aligned_malloc(size_t size, size_t alignment);

/**
 * @brief Frees memory allocated with aligned_malloc.
 *
 * On Windows, uses _aligned_free.
 * On POSIX systems, uses free.
 *
 * @param ptr Pointer to the memory to free.
 */
void aligned_free(void* ptr);

#endif // IMSDL_ALIGN_H
