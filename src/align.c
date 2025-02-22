/**
 * @file src/align.c
 * @brief Provides functions for memory alignment.
 *
 * This file contains functions to align memory to a specified alignment requirement.
 * The alignment must be a power of 2 and at least sizeof(void*).
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef _WIN32
    #include <malloc.h>
#endif

#include "logger.h"
#include "align.h"

/**
 * @brief Aligns allocated memory to the specified alignment.
 *
 * @param ptr The pointer to the memory to align.
 * @param alignment The alignment requirement, must be a power of 2 and at least sizeof(void*).
 * @return A pointer to the aligned memory, or NULL if alignment fails.
 */
static void* align_pointer(void* ptr, size_t alignment) {
    if (ptr == NULL) {
        return NULL;
    }
    if (alignment == 0 || (alignment & (alignment - 1)) == 1) {
        LOG_ERROR("Invalid alignment, must be a power of 2.\n");
        return NULL;
    }
    // Align the pointer to the specified alignment
    uintptr_t addr = (uintptr_t) ptr;
    if (addr % alignment != 0) {
        addr += alignment - (addr % alignment);
    }
    return (void*) addr;
}

/**
 * @brief Allocates aligned memory using posix_memalign.
 *
 * @param alignment The alignment requirement, must be a power of 2 and at least sizeof(void*).
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory, or NULL if allocation fails.
 */
void* align_malloc(size_t alignment, size_t size) {
    // Ensure alignment is at least sizeof(void*) and a power of 2
    if (alignment < sizeof(void*)) {
        alignment = sizeof(void*);
    }
    if ((alignment & (alignment - 1)) != 0) {
        LOG_ERROR("Alignment %zu is not a power of 2.\n", alignment);
        return NULL;
    }

    // Allocate memory with posix_memalign
    void* ptr = NULL;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        LOG_ERROR(
            "posix_memalign failed (alignment=%zu, size=%zu, error=%d).\n", alignment, size, result
        );
        return NULL;
    }

    return ptr;
}
