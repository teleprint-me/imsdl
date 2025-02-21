#include "logger.h"
#include "viewport.h"

#include <stdio.h>

// Don't over complicate this, keep this simple for now
/// @note It's just the mouse state. Probably needs a better name.
struct IMState { // maybe IMSDL_Mouse_State
    int mouse_x;
    int mouse_y;
    int mouse_down;

    int hot;
    int active;
};

int main(void) {

    // Initialize the window and opengl context.
    IMSDL_Viewport* viewport = imsdl_create_viewport("IMSDL", 800, 600, 0);
    if (!viewport) {
        LOG_ERROR("Failed to create viewport!\n");
        return 1;
    }
    /// @note Viewport must be initialized before logging.
    imsdl_log_sdl_and_opengl();
    imsdl_log_viewport(viewport);

    // Initialize the vertex buffer.
    float vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    imsdl_init_opengl_vertex_buffer(viewport, vertices, sizeof(vertices));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        imsdl_render(viewport);
    }

    // Cleanup
    imsdl_destroy_viewport(viewport);

    return 0;
}
