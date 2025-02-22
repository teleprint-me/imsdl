/**
 * @file src/arena.c
 * @brief A simple linear allocator, also known as a Arena.
 */

#include <stdlib.h>
#include <stdint.h>

#include <logger.h>
#include "arena.h"

typedef struct Arena {
    void* data; // Pointer to the allocated memory
    size_t element_size; // Size of each element in the arena
    size_t alignment; // Alignment requirement for the arena's data
    size_t size; // Number of elements currently used in the arena
} Arena;

Arena* arena_create(size_t initial_capacity, size_t element_size, size_t alignment) {
    // Check if initial capacity is valid
    if (initial_capacity <= 0) {
        LOG_ERROR("Invalid initial capacity, must be greater than 0.\n");
        return NULL;
    }

    // Check if element size is valid
    if (element_size <= 0) {
        LOG_ERROR("Invalid element size, must be greater than 0.\n");
        return NULL;
    }

    // Check if the alignment is valid (must be a power of 2)
    if (alignment % 2 != 0 || alignment < sizeof(void*)) {
        LOG_ERROR("Invalid alignment, must be a power of 2.\n");
        return NULL;
    }

    // Allocate memory for the arena structure
    Arena* arena = malloc(sizeof(Arena));
    if (arena == NULL) {
        return NULL;
    }

    // Calculate the actual capacity to align the data
    size_t aligned_capacity = (initial_capacity + alignment - 1) & ~(alignment - 1);

    arena->data = malloc(aligned_capacity);
    if (arena->data == NULL) {
        free(arena);
        return NULL;
    }

    arena->size = 0;
    arena->capacity = aligned_capacity / element_size;
    arena->element_size = element_size;
    arena->alignment = alignment;

    return arena;
}
