/**
 * @file src/align.c
 * @brief Provides functions for memory alignment.
 *
 * This file contains functions to align memory to a specified alignment requirement.
 * The alignment must be a power of 2 and at least sizeof(void*).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#ifdef _WIN32
    #include <malloc.h>
#else
    #include <errno.h>
#endif

#include "logger.h"
#include "align.h"

/**
 * @brief Aligns a pointer to the specified alignment.
 *
 * @param ptr The pointer to the memory to align.
 * @param alignment The alignment requirement, must be a power of 2 and at least sizeof(void*).
 * @return A pointer to the aligned memory, or NULL if alignment fails.
 */
static void* aligned_pointer(void* ptr, size_t alignment) {
    if (ptr == NULL) {
        return NULL;
    }
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        LOG_ERROR("%s: Invalid alignment, must be a power of 2.\n", __func__);
        return NULL;
    }

    uintptr_t addr = (uintptr_t) ptr;
    if (addr % alignment != 0) {
        addr += alignment - (addr % alignment);
    }
    return (void*) addr;
}

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
void* aligned_malloc(size_t alignment, size_t size) {
    // Ensure alignment is at least sizeof(void*) and a power of 2
    if (alignment < sizeof(void*)) {
        alignment = sizeof(void*);
    }
    if ((alignment & (alignment - 1)) != 0) {
        LOG_ERROR("%s: Alignment %zu is not a power of 2.\n", __func__, alignment);
        return NULL;
    }

    void* ptr = NULL;
#ifdef _WIN32
    ptr = _aligned_malloc(size, alignment);
    if (ptr == NULL) {
        LOG_ERROR(
            "%s: _aligned_malloc failed (alignment=%zu, size=%zu).\n", __func__, alignment, size
        );
    }
#else
    int result = posix_memalign(&ptr, alignment, size);
    if (result != 0) {
        LOG_ERROR(
            "%s: posix_memalign failed (alignment=%zu, size=%zu, error=%d).\n",
            __func__,
            alignment,
            size,
            result
        );
        return NULL;
    }
#endif

    return ptr;
}

/**
 * @brief Frees memory allocated with aligned_malloc.
 *
 * On Windows, uses _aligned_free.
 * On POSIX systems, uses free.
 *
 * @param ptr Pointer to the memory to free.
 */
void aligned_free(void* ptr) {
    if (ptr) {
#ifdef _WIN32
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }
}
