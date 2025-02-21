/**
 * @file src/viewport.c
 * @brief Create a SDL window with OpenGL support.
 * @note Avoid memory allocations as much as possible.
 */

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Define Structures
typedef struct IMSDL_Viewport_Color {
    float r, g, b, a;
} IMSDL_Viewport_Color;

typedef struct IMSDL_Viewport_GL {
    GLuint vao;
    GLuint vbo;
    SDL_GLContext context;
    int swap_interval;
} IMSDL_Viewport_GL;

typedef struct IMSDL_Viewport_View {
    SDL_Window* window;
    const char* title;
    int width;
    int height;
    int flags;
} IMSDL_Viewport_View;

typedef struct IMSDL_Viewport {
    IMSDL_Viewport_View view;
    IMSDL_Viewport_GL gl;
    IMSDL_Viewport_Color color;
} IMSDL_Viewport;

SDL_Window* imsdl_create_window(IMSDL_Viewport* viewport);
GLenum imsdl_create_opengl_context(IMSDL_Viewport* viewport);
void imsdl_init_opengl_vertex_buffer(IMSDL_Viewport* viewport, float* vertices, size_t vertex_size);
void imsdl_render(IMSDL_Viewport* viewport);
IMSDL_Viewport imsdl_create_viewport(const char* title, int width, int height, int flags);
void imsdl_destroy_viewport(IMSDL_Viewport viewport);
