#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

SDL_Window* create_window(const char* title, int width, int height) {
    SDL_Window* window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    }

    return window;
}

void draw_rectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = create_window("imsdl Window", 800, 600);
    if (!window) {
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        fprintf(stderr, "SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (SDL_GL_SetSwapInterval(1) < 0) { // Enable VSync
        fprintf(stderr, "Warning: Unable to enable VSync! %s\n", SDL_GetError());
    }

    // OpenGL coordinate setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1); // Set up 2D mode with top-left origin

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the rectangle at (100,100) with size (200,150)
        draw_rectangle(100.0f, 100.0f, 200.0f, 150.0f);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
