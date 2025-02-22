/**
 * @file src/viewport.c
 * @brief Create a SDL window with OpenGL support.
 * @note Avoid memory allocations as much as possible.
 */

#include "viewport.h"
#include "logger.h"

/**
 * @brief Initialize SDL Window
 */
void imsdl_init_sdl_window(IMSDL_Viewport* viewport) {
    // Initialize SDL Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERROR("SDL_Init Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Set window flags
    if (viewport->view.flags == 0) {
        viewport->view.flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    }

    // Create window with specified flags and dimensions
    viewport->view.window = SDL_CreateWindow(
        viewport->view.title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        viewport->view.width,
        viewport->view.height,
        viewport->view.flags
    );

    // Check if window creation failed
    if (!viewport->view.window) {
        LOG_ERROR("SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Initialize OpenGL Context
 */
void imsdl_init_opengl_context(IMSDL_Viewport* viewport) {
    // Set OpenGL version and profile
    /// @note I think hardcoding the versions is a bad idea.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create OpenGL context
    viewport->gl.context = SDL_GL_CreateContext(viewport->view.window);
    if (!viewport->gl.context) {
        LOG_ERROR("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewResult = glewInit();
    if (glewResult != GLEW_OK) {
        LOG_ERROR("GLEW Initialization Error: %s\n", glewGetErrorString(glewResult));
        exit(EXIT_FAILURE);
    }

    // Check if GLEW loaded necessary functions
    if (!GLEW_VERSION_2_0) {
        printf("OpenGL 2.0+ is required but not supported!\n");
        exit(1);
    }

    // Set swap interval for vsync
    SDL_GL_SetSwapInterval(viewport->gl.swap_interval);

    // Enable depth testing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Get error code after initialization
    int error_code = glGetError();
    if (error_code != GL_NO_ERROR) {
        LOG_ERROR("OpenGL Error: %d\n", error_code);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Initialize Vertex Buffer
 */
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

/**
 * @brief Create Viewport
 */
IMSDL_Viewport* imsdl_create_viewport(const char* title, int width, int height, int flags) {
    // Allocate memory for viewport structure
    IMSDL_Viewport* viewport = (IMSDL_Viewport*) malloc(sizeof(IMSDL_Viewport));
    if (!viewport) {
        LOG_ERROR("Failed to allocate memory for viewport\n");
        return NULL;
    }

    // Initialize viewport structure
    viewport->view.title = title;
    viewport->view.width = width;
    viewport->view.height = height;
    viewport->view.flags = flags;
    viewport->color = (IMSDL_Viewport_Color) {0.1f, 0.1f, 0.1f, 1.0f};
    viewport->gl.swap_interval = 1;

    imsdl_init_sdl_window(viewport);
    imsdl_init_opengl_context(viewport);

    return viewport;
}

/**
 * @brief Destroy Viewport
 */
void imsdl_destroy_viewport(IMSDL_Viewport* viewport) {
    if (viewport) {
        glDeleteVertexArrays(1, &viewport->gl.vao);
        glDeleteBuffers(1, &viewport->gl.vbo);

        SDL_GL_DeleteContext(viewport->gl.context);
        SDL_DestroyWindow(viewport->view.window);
        SDL_Quit();

        free(viewport);
    }
}

/**
 * @brief Render Function
 */
void imsdl_render(IMSDL_Viewport* viewport, GLuint shader_program) {
    glClearColor(viewport->color.r, viewport->color.g, viewport->color.b, viewport->color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(viewport->gl.vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0);

    SDL_GL_SwapWindow(viewport->view.window);
}

/**
 * @brief Basic Event Handling
 */
void imsdl_handle_events(int* running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = 0;
        }
    }
}

// --- SDL, OpenGL, and Viewport Logging ---

/**
 * @brief Log Viewport Information
 */
void imsdl_log_viewport(IMSDL_Viewport* viewport) {
    LOG_INFO("Viewport Title: %s\n", viewport->view.title);
    LOG_INFO("Viewport Width: %d\n", viewport->view.width);
    LOG_INFO("Viewport Height: %d\n", viewport->view.height);
    LOG_INFO("Viewport Flags: %d\n", viewport->view.flags);
    LOG_INFO(
        "Viewport Color: (%f, %f, %f, %f)\n",
        (double) viewport->color.r,
        (double) viewport->color.g,
        (double) viewport->color.b,
        (double) viewport->color.a
    );
    LOG_INFO("Viewport Swap Interval: %d\n", viewport->gl.swap_interval);
}

/**
 * @brief Log SDL and OpenGL Information
 */
void imsdl_log_sdl_and_opengl(void) {
    // Log SDL Info
    SDL_version version;
    SDL_GetVersion(&version);
    LOG_INFO("SDL Version: %d.%d.%d\n", version.major, version.minor, version.patch);
    int profile;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);
    LOG_INFO(
        "SDL OpenGL Profile: %s\n",
        (profile == SDL_GL_CONTEXT_PROFILE_CORE) ? "Core" : "Compatibility"
    );
    LOG_INFO("OpenGL Version: %s\n", glGetString(GL_VERSION));
    LOG_INFO("OpenGL Shading Language Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    LOG_INFO("GLEW Version: %s\n", glewGetString(GLEW_VERSION));
}
