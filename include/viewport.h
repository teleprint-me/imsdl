/**
 * @file include/viewport.h
 * @brief Create a SDL window with OpenGL support.
 * @note Avoid memory allocations as much as possible.
 */

#ifndef IMSDL_VIEWPORT_H
#define IMSDL_VIEWPORT_H

// Include SDL, OpenGL, and GLEW headers
#include <GL/glew.h>
#include <GL/gl.h> /// @warning Must be included after glew!
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Viewport Color
typedef struct IMSDL_Viewport_Color {
    float r, g, b, a;
} IMSDL_Viewport_Color;

// Viewport OpenGL
typedef struct IMSDL_Viewport_GL {
    GLuint vao;
    GLuint vbo;
    SDL_GLContext context;
    int swap_interval;
} IMSDL_Viewport_GL;

// Viewport SDL Window
typedef struct IMSDL_Viewport_View {
    SDL_Window* window;
    const char* title;
    int width;
    int height;
    int flags;
} IMSDL_Viewport_View;

// Viewport Structure
typedef struct IMSDL_Viewport {
    IMSDL_Viewport_View view;
    IMSDL_Viewport_GL gl;
    IMSDL_Viewport_Color color;
} IMSDL_Viewport;

// Initialize SDL Window and OpenGL Context
void imsdl_init_sdl_window(IMSDL_Viewport* viewport);
void imsdl_init_opengl_context(IMSDL_Viewport* viewport);
void imsdl_init_opengl_vertex_buffer(IMSDL_Viewport* viewport, float* vertices, size_t vertex_size);

// Create and Destroy Viewport
IMSDL_Viewport* imsdl_create_viewport(const char* title, int width, int height, int flags);
void imsdl_destroy_viewport(IMSDL_Viewport* viewport);

// Enable and disable vsync
void imsdl_toggle_vsync(IMSDL_Viewport* viewport);

// Render Function
void imsdl_render(IMSDL_Viewport* viewport, GLuint shader_program);

// Event Handling (Basic)
void imsdl_handle_events(int* running);

// --- SDL, OpenGL, and Viewport Logging ---
void imsdl_log_viewport(IMSDL_Viewport* viewport);
void imsdl_log_sdl_and_opengl(void);

#endif // IMSDL_VIEWPORT_H
