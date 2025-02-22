#include "logger.h"
#include "viewport.h"
#include "shaders.h"

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
    IMSDL_Viewport* viewport = imsdl_create_viewport("IMSDL", 800, 600, 0);
    if (!viewport) {
        LOG_ERROR("Failed to create viewport!");
        return 1;
    }
    imsdl_log_sdl_and_opengl();
    imsdl_log_viewport(viewport);

    float vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    imsdl_init_opengl_vertex_buffer(viewport, vertices, sizeof(vertices));

    GLuint shader_program
        = imsdl_create_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");

    int running = 1;
    while (running) {
        imsdl_handle_events(&running);
        imsdl_render(viewport, shader_program);
    }

    imsdl_destroy_viewport(viewport);
    return 0;
}
