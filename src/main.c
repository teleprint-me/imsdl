#include "logger.h"
#include "viewport.h"
#include "shaders.h"

#include <stdio.h>

// Don't over complicate this, keep this simple for now
typedef struct IMSDL_Mouse_State {
    int x;
    int y;
    int left_up;
    int left_down;
    int right_up;
    int right_down;
    int wheel_up;
    int wheel_down;
    int hot;
    int active;
} IMSDL_Mouse_State;

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

    IMSDL_Mouse_State mouse = {0};
    mouse.x = mouse.y = 0;
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEMOTION:
                    mouse.x = event.motion.x;
                    mouse.y = event.motion.y;
                    LOG_INFO("Mouse: x=%d, y=%d", mouse.x, mouse.y);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouse.left_down = 1;
                        LOG_INFO("Mouse left down.");
                    }
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        mouse.right_down = 1;
                        LOG_INFO("Mouse right down.");
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouse.left_up = 1;
                        LOG_INFO("Mouse left up.");
                    }
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        mouse.right_up = 1;
                        LOG_INFO("Mouse right up.");
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0) {
                        LOG_INFO("Scrolling up.");
                    } else {
                        LOG_INFO("Scrolling down.");
                    }
            }
        }
        imsdl_render(viewport, shader_program);
    }

    imsdl_destroy_viewport(viewport);
    return 0;
}
