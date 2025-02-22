/**
 * @file src/arena.c
 * @brief A simple linear allocator, also known as an Arena.
 */

#include "logger.h"
#include "align.h"
#include "arena.h"

Arena* arena_create(size_t initial_capacity, size_t element_size, size_t alignment) {
    // Ensure valid input
    if (initial_capacity == 0) {
        LOG_ERROR("Invalid initial capacity, must be greater than 0.\n");
        return NULL;
    }
    if (element_size == 0) {
        LOG_ERROR("Invalid element size, must be greater than 0.\n");
        return NULL;
    }

    // Allocate memory for the arena structure
    Arena* arena = (Arena*) malloc(sizeof(Arena));
    if (arena == NULL) {
        LOG_ERROR("Failed to allocate memory for Arena.\n");
        return NULL;
    }

    // Allocate memory for the data array with alignment
    arena->data = aligned_malloc(alignment, initial_capacity * element_size;);
    if (arena->data == NULL) {
        LOG_ERROR("Failed to allocate memory for Arena data.\n");
        free(arena);
        return NULL;
    }

    // Initialize arena fields
    arena->size = 0;
    arena->capacity = initial_capacity;
    arena->element_size = element_size;
    arena->alignment = alignment;

    return arena;
}

void arena_free(Arena* arena) {
    if (arena) {
        free(arena->data); // Free the aligned data
        free(arena); // Free the arena structure itself
    }
}
