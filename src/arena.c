/**
 * @file src/arena.c
 * @brief A simple linear allocator, also known as an Arena.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdalign.h>

#include <logger.h>
#include "arena.h"

typedef struct Arena {
    void* data; // Pointer to the allocated memory
    size_t element_size; // Size of each element in the arena
    size_t alignment; // Alignment requirement for the arena's data
    size_t capacity; // Total capacity of the arena in elements
    size_t size; // Number of elements currently used in the arena
} Arena;

// Helper function to align pointer
static void* align_pointer(void* ptr, size_t alignment) {
    uintptr_t addr = (uintptr_t) ptr;
    if (addr % alignment != 0) {
        addr += alignment - (addr % alignment);
    }
    return (void*) addr;
}

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
    if (alignment == 0 || (alignment & (alignment - 1)) == 1) {
        LOG_ERROR("Invalid alignment, must be a power of 2.\n");
        return NULL;
    }

    // Allocate memory for the arena structure
    Arena* arena = (Arena*) malloc(sizeof(Arena));
    if (arena == NULL) {
        LOG_ERROR("Failed to allocate memory for Arena.\n");
        return NULL;
    }

    // Allocate extra space to ensure alignment
    void* raw_data = malloc(initial_capacity * element_size + alignment;);
    if (raw_data == NULL) {
        LOG_ERROR("Failed to allocate memory for Arena data.\n");
        free(arena);
        return NULL;
    }

    // Align the data pointer
    arena->data = (void*) align_pointer(raw_data, alignment);
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
