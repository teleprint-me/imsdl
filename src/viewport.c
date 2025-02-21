/**
 * @file src/viewport.c
 * @brief Create a SDL window with OpenGL support.
 * @note Avoid memory allocations as much as possible.
 */

#include "viewport.h"
#include "logger.h"

// Create a SDL window for the viewport
void imsdl_init_sdl_window(IMSDL_Viewport* viewport) {
    if (viewport->view.flags == 0) {
        viewport->view.flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    }

    SDL_Window* window = SDL_CreateWindow(
        viewport->view.title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        viewport->view.width,
        viewport->view.height,
        viewport->view.flags
    );

    if (!window) {
        LOG_ERROR("SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    viewport->view.window = window;
}

// OpenGL Context Creation and Initialization
void imsdl_init_opengl_context(IMSDL_Viewport* viewport) {
    // Create OpenGL context for the window
    viewport->gl.context = SDL_GL_CreateContext(viewport->view.window);
    if (!viewport->gl.context) {
        LOG_ERROR("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Set OpenGL version and profile (Core Profile)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewResult = glewInit();
    if (glewResult != GLEW_OK) {
        LOG_ERROR("GLEW Initialization Error: %s\n", glewGetErrorString(glewResult));
        exit(EXIT_FAILURE);
    }

    SDL_GL_SetSwapInterval(viewport->gl.swap_interval);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Get error code after initialization
    int error_code = glGetError();
    if (error_code != GL_NO_ERROR) {
        LOG_ERROR("OpenGL Error: %s\n", gluErrorString(error_code));
        exit(EXIT_FAILURE);
    }
}

// Vertex Buffer Initialization
void imsdl_init_opengl_vertex_buffer(
    IMSDL_Viewport* viewport, float* vertices, size_t vertex_size
) {
    glGenVertexArrays(1, &viewport->gl.vao);
    glBindVertexArray(viewport->gl.vao);

    glGenBuffers(1, &viewport->gl.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, viewport->gl.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_size, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

IMSDL_Viewport* imsdl_create_viewport(const char* title, int width, int height, int flags) {
    // Allocate memory for the viewport structure
    IMSDL_Viewport* viewport = (IMSDL_Viewport*) malloc(sizeof(IMSDL_Viewport));
    if (!viewport) {
        LOG_ERROR("Failed to allocate memory for viewport\n");
        return NULL; // Return NULL on error
    }
    // Initialize the viewport structure
    viewport->view.title = title;
    viewport->view.width = width;
    viewport->view.height = height;
    viewport->view.flags = flags;
    viewport->color = (IMSDL_Viewport_Color) {0.1f, 0.1f, 0.1f, 1.0f};

    // Set up the OpenGL context
    viewport->gl.swap_interval = 1; // Enable vsync by default
    imsdl_init_opengl_context(viewport); // Initialize OpenGL context

    // Create the SDL window and OpenGL context
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERROR("Failed to initialize SDL: %s\n", SDL_GetError());
        return NULL; // Return NULL on error
    }
    imsdl_init_sdl_window(viewport); // Initialize SDL window

    return viewport; // Return the initialized viewport
}

void imsdl_toggle_vsync(IMSDL_Viewport* viewport) {
    viewport->gl.swap_interval = viewport->gl.swap_interval == 1 ? 0 : 1;
    SDL_GL_SetSwapInterval(viewport->gl.swap_interval);
}

void imsdl_render(IMSDL_Viewport* viewport) {
    glClearColor(viewport->color.r, viewport->color.g, viewport->color.b, viewport->color.a);
    glViewport(0, 0, viewport->view.width, viewport->view.height);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, viewport->gl.vbo);
    glDrawArrays(GL_QUADS, 0, 4);
    SDL_GL_SwapWindow(viewport->view.window);
}
