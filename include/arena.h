/**
 * @file include/arena.h
 * @brief A simple linear allocator, also known as an Arena.
 */

#ifndef IMSDL_ARENA_H
#define IMSDL_ARENA_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdalign.h>

/**
 * @struct Arena
 * @brief A simple linear allocator, also known as an Arena.
 */
typedef struct Arena {
    void* data; // Pointer to the allocated memory
    size_t element_size; // Size of each element in the arena
    size_t alignment; // Alignment requirement for the arena's data
    size_t capacity; // Total capacity of the arena in elements
    size_t size; // Number of elements currently used in the arena
} Arena;

Arena* arena_create(size_t initial_capacity, size_t element_size, size_t alignment);
void arena_free(Arena* arena);

#endif // IMSDL_ARENA_H
